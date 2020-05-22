/**
 * Beast Brawl
 * Game created as a final project of the Multimedia Engineering Degree in the University of Alicante.
 * Made by Clover Games Studio, with members 
 * Carlos de la Fuente Torres delafuentetorresc@gmail.com,
 * Antonio Jose Martinez Garcia https://www.linkedin.com/in/antonio-jose-martinez-garcia/,
 * Jesús Mas Carretero jmasc03@gmail.com, 
 * Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/, 
 * Rubén Rubio Martínez https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/, 
 * and Jose Valdés Sirvent https://www.linkedin.com/in/jose-f-valdés-sirvent-6058b5a5/ github -> josefrvaldes
 * 
 * 
 * @author Carlos de la Fuente Torres <delafuentetorresc@gmail.com>
 * @author Jose Valdés Sirvent
 * 
 */
 
 
 
#include "CLPhysics.h"
#include <iostream>
#include <memory>
#include "../Aliases.h"
#include "../Components/CBoundingChassis.h"
#include "../Components/CBoundingCilindre.h"
#include "../Components/CBoundingOBB.h"
#include "../Components/CExternalForce.h"
#include "../Components/CIDOnline.h"
#include "../Components/COnline.h"
#include "../Components/CHurt.h"
#include "../Entities/BoundingOBB.h"
#include "../Entities/PowerUp.h"
#include "../EventManager/Event.h"
#include "../EventManager/EventManager.h"
#include "../Managers/ManBoundingOBB.h"

#include <Components/CAABoundingBox.h>
#include <Components/CBoundingPlane.h>
#include <Components/CBoundingSphere.h>
#include <Components/CCar.h>
#include <Components/CId.h>
#include <Components/CNitro.h>
#include <Components/CTotem.h>
#include <Components/CTransformable.h>
#include <Entities/BoundingWall.h>
#include <Entities/Car.h>
#include <Entities/CarAI.h>
#include <Entities/CarHuman.h>
#include <Entities/Camera.h>
#include <EventManager/Event.h>
#include <EventManager/EventManager.h>
#include <Managers/ManBoundingGround.h>
#include <Managers/ManBoundingWall.h>
#include <Managers/ManBoxPowerUp.h>
#include <Managers/ManCar.h>
#include <Managers/ManNavMesh.h>
#include <Managers/ManPowerUp.h>
#include <Managers/ManTotem.h>
#include <Managers/ManCamera.h>
#include <Managers/Manager.h>
#include <Systems/Utils.h>
#include "../Components/CGravity.h"
#include "../Components/CRemovableObject.h"
#include "../Components/CShield.h"
#include "../Components/CCamera.h"
#include "../Facade/Render/RenderFacadeIrrlicht.h"
#include "../Facade/Render/RenderFacadeManager.h"
#include "../Constants.h"

#include <cmath>

using namespace std;

CLPhysics::CLPhysics() {
    SubscribeToEvents();
}

void CLPhysics::AddManager(Manager &m) {
    managers.push_back(&m);
}
/*
* [0] manCars
* [1] manWalls
* [2] manOBB
* [3] manGround
* [4] manPowerUps
* [5] manNavMesh
* [6] manTotem
*/
//const auto &manCars = managers[0];
void CLPhysics::Update(float delta) {
    Simulate(delta);

    // Aplicamos las fisicas de gravedad
    RepositionBounding();
    CentralSystemGravity();

    // Aplicamos las fisicas de colision
    RepositionBounding();
    CentralSystemCollisions();
}

void CLPhysics::SubscribeToEvents() {
    //cout << "Vamos a suscribir a los eventos al clPhysics" << endl;
    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::NEW_CRASH_PU_CAR_RECEIVED,
        bind(&CLPhysics::NewCrashPUCarReceived, this, placeholders::_1),
        "CreatePowerUp"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::NEW_CRASH_PU_WALL_RECEIVED,
        bind(&CLPhysics::NewCrashPUWallReceived, this, placeholders::_1),
        "CreatePowerUp"));
}

void CLPhysics::NewCrashPUWallReceived(DataMap *d) {
    uint16_t idPU = any_cast<uint16_t>((*d)[DataType::ID_PU]);
    const auto &manPowerUps = managers[4];

    for (const auto &currentPU : manPowerUps->GetEntities()) {
        CIDOnline *cOnline = static_cast<CIDOnline *>(currentPU->GetComponent(CompType::CIDOnlineComp).get());
        if (cOnline->idOnline == idPU) {
            auto cRemovableObj = static_cast<CRemovableObject *>(currentPU->GetComponent(CompType::RemovableObjectComp).get());
            cRemovableObj->destroy = true;
            break;
        }
    }
}

void CLPhysics::NewCrashPUCarReceived(DataMap *d) {
    //cout << "Hemos recibido un NewCrashPUCarReceived 2" << endl;
    uint16_t idCar = any_cast<uint16_t>((*d)[DataType::ID_CAR]);
    uint16_t idPU = any_cast<uint16_t>((*d)[DataType::ID_PU]);
    const auto &manCars = managers[0];
    const auto &manPowerUps = managers[4];
    Entity *carFound = nullptr;
    for (const auto &currentCar : manCars->GetEntities()) {
        COnline *cOnline = static_cast<COnline *>(currentCar->GetComponent(CompType::OnlineComp).get());
        if (cOnline->idClient == idCar) {
            carFound = currentCar.get();
            break;
        }
    }
    if(carFound == nullptr) {
        //cout << "Nos salimos del NewCrashPUCarReceived porque no encontramos el coche" << endl;
        return;
    }

    PowerUp *puFound = nullptr;
    for (const auto &currentPU : manPowerUps->GetEntities()) {
        CIDOnline *cOnline = static_cast<CIDOnline *>(currentPU->GetComponent(CompType::CIDOnlineComp).get());
        if (cOnline->idOnline == idPU) {
            puFound = static_cast<PowerUp *>(currentPU.get());
            break;
        }
    }
    if (puFound==nullptr) {
        //cout << "Nos salimos del NewCrashPUCarReceived porque no encontramos el PU" << endl;
        return;
    }
    HandleCollisionPUWithCar(puFound, carFound);
}

void CLPhysics::CentralSystemGravity() {
    ConstGravity();
    //aplicateGravity();
    // COLISIONES ORDENADAS POR LA COORDENADA Y (DE MAYOR A MENOR)
    HandleCollisionsWithGround();
    LimitRotationCarY();
}


void CLPhysics::ConstGravity() {
    ManCar *manCar = static_cast<ManCar *>(managers[0]);
    for (const auto &car : manCar->GetEntities()) {
        CBoundingChassis *chaCar = static_cast<CBoundingChassis *>(car->GetComponent(CompType::CompBoundingChassis).get());
        CTransformable *trcar = static_cast<CTransformable *>(car->GetComponent(CompType::TransformableComp).get());
        chaCar->sphereBehind->center.y += gravityCar;
        chaCar->sphereFront->center.y += gravityCar;
        RePositionCarY(*trcar, *chaCar->sphereBehind, *chaCar->sphereFront);
    }
    ManPowerUp *manPowerUp = static_cast<ManPowerUp *>(managers[4]);
    for (const auto &powerUp : manPowerUp->GetEntities()) {
        CBoundingSphere *spEntity = static_cast<CBoundingSphere *>(powerUp->GetComponent(CompType::CompBoundingSphere).get());
        CTransformable *trEntity = static_cast<CTransformable *>(powerUp->GetComponent(CompType::TransformableComp).get());
        spEntity->center.y += gravityPU;
        RePositionEntityY(*trEntity, *spEntity);
    }
    ManTotem *manTotem = static_cast<ManTotem *>(managers[7]);
    const auto &totem = manTotem->GetEntities()[0];
    auto cTotem = static_cast<CTotem*>(totem->GetComponent(CompType::TotemComp).get());
    CBoundingSphere *spTotem = static_cast<CBoundingSphere *>(totem->GetComponent(CompType::CompBoundingSphere).get());
    CTransformable *trTotem = static_cast<CTransformable *>(totem->GetComponent(CompType::TransformableComp).get());
    if(cTotem->speed < cTotem->MaxSpeed*0.3){
        spTotem->center.y -= gravityPU;   
    }else{
        spTotem->center.y += gravityPU;
    }
    RePositionEntityY(*trTotem, *spTotem);
}

void CLPhysics::aplicateGravity() {
    ManCar *manCar = static_cast<ManCar *>(managers[0]);
    const auto &cars = manCar->GetEntities();
    size_t numCar = cars.size();
    for (size_t currentCar = 0; currentCar < numCar; currentCar++) {
        Entity *car = manCar->GetEntities()[currentCar].get();
        CBoundingChassis *chaCar = static_cast<CBoundingChassis *>(car->GetComponent(CompType::CompBoundingChassis).get());
        CTransformable *trcar = static_cast<CTransformable *>(car->GetComponent(CompType::TransformableComp).get());
        CGravity *gravCar = static_cast<CGravity *>(car->GetComponent(CompType::CompGravity).get());
        gravCar->currentGravity += -0.1;
        if (gravCar->currentGravity < -3.0)
            gravCar->currentGravity = -3.0;
        chaCar->sphereBehind->center.y += gravCar->currentGravity;
        chaCar->sphereFront->center.y += gravCar->currentGravity;
        RePositionCarY(*trcar, *chaCar->sphereBehind, *chaCar->sphereFront);
    }
}

void CLPhysics::HandleCollisionsWithGround() {
    ManCar *manCar = static_cast<ManCar *>(managers[0]);
    ManBoundingGround *manGrounds = static_cast<ManBoundingGround *>(managers[3]);

    const auto &cars = manCar->GetEntities();
    size_t numCar = cars.size();

    auto grounds = manGrounds->GetEntities();
    size_t numGrounds = grounds.size();

    // los coches con los grounds
    for (size_t currentCar = 0; currentCar < numCar; currentCar++) {
        //bool sameGround = false;
        Entity *car = manCar->GetEntities()[currentCar].get();
        CBoundingPlane *planeSphereBehind = nullptr;
        CBoundingPlane *planeSphereFront = nullptr;
        CBoundingChassis *chaCar = static_cast<CBoundingChassis *>(car->GetComponent(CompType::CompBoundingChassis).get());
        CBoundingSphere *spBehindCar = chaCar->sphereBehind.get();
        CBoundingSphere *spFrontCar = chaCar->sphereFront.get();
        CTransformable *trcar = static_cast<CTransformable *>(car->GetComponent(CompType::TransformableComp).get());

        bool interBeh = false;
        for (size_t currentGround = 0; currentGround < numGrounds && !interBeh; currentGround++) {
            BoundingWall *ground = static_cast<BoundingWall *>(grounds[currentGround].get());
            CBoundingPlane *plane = static_cast<CBoundingPlane *>(ground->GetComponent(CompType::CompBoundingPlane).get());
            interBeh = CollisionsSphereGround(*trcar, *spBehindCar, *plane);
            if (interBeh) {
                planeSphereBehind = plane;
                //cout << " EL BEHIND COLISIONA CON EL PLANO: " << currentGround << endl;
            }
        }
        bool interFro = false;
        for (size_t currentGround = 0; currentGround < numGrounds && !interFro; currentGround++) {
            BoundingWall *ground = static_cast<BoundingWall *>(grounds[currentGround].get());
            CBoundingPlane *plane = static_cast<CBoundingPlane *>(ground->GetComponent(CompType::CompBoundingPlane).get());
            interFro = CollisionsSphereGround(*trcar, *spFrontCar, *plane);
            if (interFro) {
                planeSphereFront = plane;
                //cout << " EL FRONT COLISIONA CON EL PLANO: " << currentGround << endl;
            }
        }
        RePositionCarY(*trcar, *spBehindCar, *spFrontCar);
        if (planeSphereFront && planeSphereBehind) {
            RotateCarXZ(*trcar, *chaCar, planeSphereBehind, planeSphereFront);
        }
    }

    // POWER-UPS
    ManCar *manPowerUp = static_cast<ManCar *>(managers[4]);
    for (auto currentPU : manPowerUp->GetEntities()) {
        CBoundingSphere *cSphere = static_cast<CBoundingSphere *>(currentPU->GetComponent(CompType::CompBoundingSphere).get());
        CTransformable *trEntity = static_cast<CTransformable *>(currentPU->GetComponent(CompType::TransformableComp).get());

        for (size_t currentGround = 0; currentGround < numGrounds; currentGround++) {
            BoundingWall *ground = static_cast<BoundingWall *>(grounds[currentGround].get());
            CBoundingPlane *plane = static_cast<CBoundingPlane *>(ground->GetComponent(CompType::CompBoundingPlane).get());
            CollisionsSphereGround(*trEntity, *cSphere, *plane);
            RePositionPUY(*trEntity, *cSphere);
        }
    }

    // TOTEM
    ManTotem *manTotem = static_cast<ManTotem *>(managers[7]);
    const auto &totem = manTotem->GetEntities()[0];
    CBoundingSphere *spTotem = static_cast<CBoundingSphere *>(totem->GetComponent(CompType::CompBoundingSphere).get());
    CTransformable *trTotem = static_cast<CTransformable *>(totem->GetComponent(CompType::TransformableComp).get());
    for (size_t currentGround = 0; currentGround < numGrounds; currentGround++) {
        BoundingWall *ground = static_cast<BoundingWall *>(grounds[currentGround].get());
        CBoundingPlane *plane = static_cast<CBoundingPlane *>(ground->GetComponent(CompType::CompBoundingPlane).get());
        CollisionsSphereGround(*trTotem, *spTotem, *plane);
        RePositionEntityY(*trTotem, *spTotem);
    }
}

void CLPhysics::LimitRotationCarY() const {
    ManCar *manCar = static_cast<ManCar *>(managers[0]);
    const auto &cars = manCar->GetEntities();
    size_t numCar = cars.size();
    for (size_t currentCar = 0; currentCar < numCar; currentCar++) {
        Entity *car = manCar->GetEntities()[currentCar].get();
        CBoundingChassis *chaCar = static_cast<CBoundingChassis *>(car->GetComponent(CompType::CompBoundingChassis).get());
        CTransformable *trcar = static_cast<CTransformable *>(car->GetComponent(CompType::TransformableComp).get());
        glm::vec3 vecDirCar = glm::vec3((chaCar->sphereFront->center.x - chaCar->sphereBehind->center.x), (chaCar->sphereFront->center.y - chaCar->sphereBehind->center.y), (chaCar->sphereFront->center.z - chaCar->sphereBehind->center.z));
        glm::vec3 vecDirEjeY = glm::vec3((chaCar->sphereFront->center.x - chaCar->sphereBehind->center.x), 0, (chaCar->sphereFront->center.z - chaCar->sphereBehind->center.z));
        double anguloo = Angle2Vectors(vecDirCar, vecDirEjeY);
        int anguloEntero = int(anguloo);
        if (chaCar->sphereFront->center.y < chaCar->sphereBehind->center.y)
            anguloEntero = -1 * anguloEntero;
        // TODO: Solo el Front no lo podemos limitar, deben ser lso dos porque puedo ir marcha atras
        if (anguloEntero < -40) {
            chaCar->sphereFront->center.y -= gravityCar;
        }
        if (anguloEntero > 40) {
            chaCar->sphereBehind->center.y -= gravityCar;
        }
        RePositionCarY(*trcar, *chaCar->sphereBehind, *chaCar->sphereFront);
    }
}

void CLPhysics::RotateCarXZ(CTransformable &trcar, CBoundingChassis &chaCar, CBoundingPlane *pl1Car, CBoundingPlane *pl2Car) const{

    auto normalPlane = pl1Car->normalizedNormal;
    if (pl1Car != pl2Car) {
        auto normalPlane1 = pl1Car->normalizedNormal;
        auto normalPlane2 = pl2Car->normalizedNormal;
        normalPlane = glm::vec3((normalPlane1.x + normalPlane2.x), (normalPlane1.y + normalPlane2.y), (normalPlane1.z + normalPlane2.z));
    }
    // rotacion en X
    auto angleRotate = Angle2Vectors(normalPlane, vec3(0, 1, 0));
    auto dirRotateY = (trcar.rotation.y * M_PI) / 180.0;
    //cout << "LA NORMAL DEL PLANO ES: (" << normalPlane.x << " , " << normalPlane.y << " , " << normalPlane.z << " )" << endl;
    auto total = abs(normalPlane.x * 100) + abs(normalPlane.z * 100);
    if (total == 0.0) total = 1.0;
    auto newR_X = sin(dirRotateY) * (normalPlane.x * 100);
    auto resultante_X = newR_X * 100 / (total);
    auto rotationX_exeX = angleRotate * (resultante_X / 100);

    auto newR_Z = cos(dirRotateY) * (normalPlane.z * 100);
    auto resultante_Z = newR_Z * 100 / total;
    auto rotationX_exeZ = angleRotate * (resultante_Z / 100);

    auto velRot = 130.0 * Constants::DELTA_TIME;

    auto rotationFinalX = rotationX_exeX + rotationX_exeZ;
    if(abs(trcar.rotation.x - rotationFinalX) > velRot ){  // deberia de multiplicarse por el delta
        if( trcar.rotation.x < rotationFinalX)
            trcar.rotation.x += velRot;
        else
            trcar.rotation.x -= velRot;
    }else{
        trcar.rotation.x = rotationFinalX;
    }

    // rotacion en Z
    auto newRz_X = -cos(dirRotateY) * (normalPlane.x*100);
    auto resultantez_X = newRz_X*100/(total);
    auto rotationZ_exeX = angleRotate*(resultantez_X/100);

    auto newRz_Z = -sin(dirRotateY) * (normalPlane.z*100);
    auto resultantez_Z = newRz_Z*100/total;
    auto rotationZ_exeZ = angleRotate*(resultantez_Z/100);


    auto rotationFinalZ = rotationZ_exeX + rotationZ_exeZ;
    if(abs(trcar.rotation.z - rotationFinalZ) > velRot ){  // deberia de multiplicarse por el delta
        if( trcar.rotation.z < rotationFinalZ)
            trcar.rotation.z += velRot;
        else
            trcar.rotation.z-= velRot;
    }else{
        trcar.rotation.z = rotationFinalZ;
    }
}


void CLPhysics::RePositionCarY(CTransformable &trCar, CBoundingSphere &sp1Car, CBoundingSphere &sp2Car) const {
    trCar.position.y = ((sp1Car.center.y + sp2Car.center.y) / 2) - sp1Car.radiusFloor;
}
void CLPhysics::RePositionEntityY(CTransformable &trEntity, CBoundingSphere &sphere) const {
    trEntity.position.y = sphere.center.y;
}
void CLPhysics::RePositionPUY(CTransformable &trEntity, CBoundingSphere &sphere) const {
    trEntity.position.y = sphere.center.y - sphere.radiusFloor;
}

bool CLPhysics::CollisionsSphereGround(CTransformable &trCar, CBoundingSphere &spCar, CBoundingPlane &plane) {
    IntersectData intersData = plane.IntersectSphereCenter(spCar);
    if (intersData.intersects) {
        SeparateSphereGround(intersData, trCar, spCar, plane);
        return true;
    }
    return false;
}

void CLPhysics::SeparateSphereGround(IntersectData &intersData, CTransformable &trCar1, CBoundingSphere &spCar1, CBoundingPlane &plane) const {
    //cout << "NO SEPARO" << endl;
    vec3 direction = spCar1.center - plane.normal;  // te da la dirección al otro bounding en x, y, z, es decir, si tenemos 200, 10, 30, significa que estamos a 200 de distancia en x, a 10 en y y a 30 en z
    vec3 nuevaDirectionCar1 = -normalize(direction);
    float correctedDistance = intersData.GetDistance();
    spCar1.center.y += nuevaDirectionCar1.y * correctedDistance;
}

void CLPhysics::CentralSystemCollisions() {
    HandleCollisions();            // COLISIONES ENTRE COCHES
    HandleCollisionsWithPlanes();  // COLISIONES COCHES PLANOS
    HandleCollisionsWithOBB();     // COLISIONES COCHES OBB
}

void CLPhysics::RepositionBounding() {
    ManCar *manCar = static_cast<ManCar *>(managers[0]);
    const auto &entities = manCar->GetEntities();
    size_t numEntities = entities.size();
    for (size_t i = 0; i < numEntities; i++) {
        Entity *car = manCar->GetEntities()[i].get();
        CTransformable *trcar = static_cast<CTransformable *>(car->GetComponent(CompType::TransformableComp).get());
        CBoundingChassis *cBoundingChassis = static_cast<CBoundingChassis *>(car->GetComponent(CompType::CompBoundingChassis).get());
        CBoundingSphere *cBoundingSphere = static_cast<CBoundingSphere *>(car->GetComponent(CompType::CompBoundingSphere).get());
        PositionSphBehindIntoTransf(*trcar, *cBoundingChassis->sphereBehind);
        PositionSphFrontIntoTransf(*trcar, *cBoundingChassis->sphereFront);
        PositionSphereIntoTransformable(*trcar, *cBoundingSphere);
        PositionCilindreIntoSpheres(*cBoundingChassis);
    }
    // POWER UPS
    ManPowerUp *manPowerUp = static_cast<ManPowerUp *>(managers[4]);
    for (auto currentPU : manPowerUp->GetEntities()) {
        CTransformable *trPU = static_cast<CTransformable *>(currentPU->GetComponent(CompType::TransformableComp).get());
        CBoundingSphere *cBoundingSphere = static_cast<CBoundingSphere *>(currentPU->GetComponent(CompType::CompBoundingSphere).get());
        cBoundingSphere->center = trPU->position;
    }
    // TOTEM
    ManTotem *manTotem = static_cast<ManTotem *>(managers[7]);
    const auto &totem = manTotem->GetEntities()[0];
    CBoundingSphere *spTotem = static_cast<CBoundingSphere *>(totem->GetComponent(CompType::CompBoundingSphere).get());
    CTransformable *trTotem = static_cast<CTransformable *>(totem->GetComponent(CompType::TransformableComp).get());
    spTotem->center = trTotem->position;
}

void CLPhysics::HandleCollisionsWithOBB() {
    ManCar *manCar = static_cast<ManCar *>(managers[0]);
    ManBoundingOBB *manOBBs = static_cast<ManBoundingOBB *>(managers[2]);

    vector<shared_ptr<Entity>> carAIs = manCar->GetEntities();
    size_t numCar = carAIs.size();

    vector<shared_ptr<Entity>> obbs = manOBBs->GetEntities();
    size_t numOBBs = obbs.size();

    //Entity* hola;

    // los coches con los OBB
    for (size_t currentAI = 0; currentAI < numCar; currentAI++) {
        Entity *car = manCar->GetEntities()[currentAI].get();
        CBoundingSphere *spcar = static_cast<CBoundingSphere *>(car->GetComponent(CompType::CompBoundingSphere).get());
        CTransformable *trcar = static_cast<CTransformable *>(car->GetComponent(CompType::TransformableComp).get());
        CId *cId = static_cast<CId *>(car->GetComponent(CompType::IdComp).get());
        CCar *ccarcar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());
        for (size_t currentOBB = 0; currentOBB < numOBBs; currentOBB++) {
            BoundingOBB *obbActual = static_cast<BoundingOBB *>(obbs[currentOBB].get());
            CBoundingOBB *cOBBactual = static_cast<CBoundingOBB *>(obbActual->GetComponent(CompType::CompBoundingOBB).get());
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            PositionSphereIntoTransformable(*trcar, *spcar);
            auto cspeed = ccarcar->speed;
            if ( HandleCollisions(*trcar, *spcar, *ccarcar, false, *cOBBactual) ) {
                if (cspeed >= 100) {
                    // Sonido choque
                    shared_ptr<DataMap> dataSound = make_shared<DataMap>();
                    (*dataSound)[ID] = cId->id;
                    (*dataSound)[VEC3_POS] = trcar->position;
                    EventManager::GetInstance().AddEventMulti(Event{CRASH_WALL, dataSound});
                }
            }
        }
    }
}

void CLPhysics::HandleCollisionsWithPlanes() {
    ManCar *manCar = static_cast<ManCar *>(managers[0]);
    ManBoundingWall *manWalls = static_cast<ManBoundingWall *>(managers[1]);

    const auto &carAIs = manCar->GetEntities();
    size_t numCar = carAIs.size();

    auto walls = manWalls->GetEntities();
    size_t numWalls = walls.size();

    // los coches con los walls
    for (size_t currentAI = 0; currentAI < numCar; currentAI++) {
        Entity *car = manCar->GetEntities()[currentAI].get();
        //CBoundingSphere *spcar = static_cast<CBoundingSphere *>(car->GetComponent(CompType::CompBoundingSphere).get());
        CBoundingChassis *chaCar = static_cast<CBoundingChassis *>(car->GetComponent(CompType::CompBoundingChassis).get());
        CTransformable *trcar = static_cast<CTransformable *>(car->GetComponent(CompType::TransformableComp).get());
        CId *cId = static_cast<CId *>(car->GetComponent(CompType::IdComp).get());
        CCar *ccarcar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());
        for (size_t currentWall = 0; currentWall < numWalls; currentWall++) {
            BoundingWall *wall = static_cast<BoundingWall *>(walls[currentWall].get());
            CBoundingPlane *plane = static_cast<CBoundingPlane *>(wall->GetComponent(CompType::CompBoundingPlane).get());

            //HandleCollisions(*trcar, *spcar, *ccarcar, false, *plane);
            auto cspeed = ccarcar->speed;
            if ( CollisionsSpherePlane(*trcar, *chaCar, *ccarcar, false, *plane) ) {
                // TODO: JUDITH -> NO PONGAS VALORES FIJOS ---> AREEGLAR ESTA PARTE
                //  cspeed >= ccarcar->maxSpeed/2       es eso lo que quieres? en vez de poner 100, osea es la mitad?
                if (cspeed >= 100) {
                    // Sonido choque
                    shared_ptr<DataMap> dataSound = make_shared<DataMap>();
                    (*dataSound)[ID] = cId->id;
                    (*dataSound)[VEC3_POS] = trcar->position;
                    EventManager::GetInstance().AddEventMulti(Event{CRASH_WALL, dataSound});
                }
            }
        }
    }
}

void CLPhysics::Simulate(float delta) {
    for (uint16_t i = 0; i < managers.size(); i++) {
        Manager *man = managers[i];
        man->Integrate(delta);
    }
}

void CLPhysics::HandleCollisions() {

    shared_ptr<DataMap> dataSound = make_shared<DataMap>();

    auto *manCar = static_cast<ManCar *>(managers[0]);

    const auto &entities = manCar->GetEntities();
    size_t numEntities = entities.size();

    // los coches entre si
    for (size_t i = 0; i < numEntities; i++) {
        Entity *car1 = manCar->GetEntities()[i].get();
        CBoundingChassis *chacar1 = static_cast<CBoundingChassis *>(car1->GetComponent(CompType::CompBoundingChassis).get());
        CTransformable *trcar1 = static_cast<CTransformable *>(car1->GetComponent(CompType::TransformableComp).get());
        CCar *ccarcar1 = static_cast<CCar *>(car1->GetComponent(CompType::CarComp).get());
        CExternalForce *cExternalForce1 = static_cast<CExternalForce *>(car1->GetComponent(CompType::CompExternalForce).get());
        for (size_t j = i + 1; j < numEntities; j++) {
            Entity *car2 = manCar->GetEntities()[j].get();
            CBoundingChassis *chacar2 = static_cast<CBoundingChassis *>(car2->GetComponent(CompType::CompBoundingChassis).get());
            CTransformable *trcar2 = static_cast<CTransformable *>(car2->GetComponent(CompType::TransformableComp).get());
            CCar *ccarcar2 = static_cast<CCar *>(car2->GetComponent(CompType::CarComp).get());
            CExternalForce *cExternalForce2 = static_cast<CExternalForce *>(car2->GetComponent(CompType::CompExternalForce).get());

            // En nuestra forma de CAPSULA tenemos 1 cilindro y 2 esferas, primero comprobamos las colisiones con el cilindro y luego con las esferas
            // CILINDRO
            bool intersect = false;
            intersect = CollisionsCilindreSphere(*trcar1, *ccarcar1, *chacar1, *trcar2, *ccarcar2, *chacar2, *cExternalForce1, *cExternalForce2);
            if (!intersect) {
                // ESFERAS
                intersect = CollisionsSphereSphere(*trcar1, *chacar1, *ccarcar1, false, *trcar2, *chacar2, *ccarcar2, *cExternalForce1, *cExternalForce2);
            }
            if (intersect) {
                checkCollisionNitro(car1, car2);
                // Sonido choque
                auto cId1 = static_cast<CId*>(car1->GetComponent(CompType::IdComp).get());
                auto cId2 = static_cast<CId*>(car2->GetComponent(CompType::IdComp).get());
                auto cMainId = static_cast<CId*>(manCar->GetCar()->GetComponent(CompType::IdComp).get());
                auto cMainSpeed = static_cast<CCar*>(manCar->GetCar()->GetComponent(CompType::CarComp).get());
                if(cId1 && cId2 && cMainId && cMainSpeed) {
                    (*dataSound)[ID] = cId1->id;
                    (*dataSound)[VEC3_POS] = trcar1->position;
                    (*dataSound)[MAIN_CAR] = false;
                    if ((cId1->id == cMainId->id || cId2->id == cMainId->id) && cMainSpeed->speed > 150 && cMainSpeed->speed < 230) {
                        (*dataSound)[MAIN_CAR] = true;
                        //cout << "EL MAIN CAR IBA A: " << cMainSpeed->speed << endl;
                    }
                    EventManager::GetInstance().AddEventMulti(Event{CRASH, dataSound});
                }
            }
        }
    }
}

void CLPhysics::checkCollisionNitro(Entity *car1, Entity *car2) {
    //vamos a comprobar si una de las dos llevaba el nitro y en cuyo caso si la otra el totem para quitarselo
    auto cNitroCar1 = static_cast<CNitro *>(car1->GetComponent(CompType::NitroComp).get());
    if (cNitroCar1->activePowerUp) {
        // si el primero tiene super nitro vemos si elsegundo tiene el totem
        auto cTotemCar2 = static_cast<CTotem *>(car2->GetComponent(CompType::TotemComp).get());
        if (cTotemCar2->active) {
            // si que lo tiene... lanzamos evento para intercambiarlo
            shared_ptr<DataMap> data = make_shared<DataMap>();
            (*data)[CAR_WITH_TOTEM] = car2;
            (*data)[CAR_WITHOUT_TOTEM] = car1;
            EventManager::GetInstance().AddEventMulti(Event{EventType::CHANGE_TOTEM_CAR, data});
        }

    } else {
        auto cNitroCar2 = static_cast<CNitro *>(car2->GetComponent(CompType::NitroComp).get());
        if (cNitroCar2->activePowerUp) {
            auto cTotemCar1 = static_cast<CTotem *>(car1->GetComponent(CompType::TotemComp).get());
            if (cTotemCar1->active) {
                // si que lo tiene... lanzamos evento para intercambiarlo
                shared_ptr<DataMap> data = make_shared<DataMap>();

                (*data)[CAR_WITH_TOTEM] = car1;
                (*data)[CAR_WITHOUT_TOTEM] = car2;
                EventManager::GetInstance().AddEventMulti(Event{EventType::CHANGE_TOTEM_CAR, data});
            }
        }
    }
}

void CLPhysics::PositionSphereIntoTransformable(CTransformable &tr, CBoundingSphere &sp) const {
    sp.center = tr.position;
    float x = -cos(Utils::DegToRad(tr.rotation.y)) * (sp.radius);
    float z = sin(Utils::DegToRad(tr.rotation.y)) * (sp.radius);
    sp.center.x += x;
    sp.center.z += z;
}

void CLPhysics::PositionSphBehindIntoTransf(CTransformable &tr, CBoundingSphere &sp) const {
    sp.center.x = tr.position.x;
    sp.center.z = tr.position.z;
    float x = -cos(Utils::DegToRad(tr.rotation.y)) * (-sp.disCenter);
    float z = sin(Utils::DegToRad(tr.rotation.y)) * (-sp.disCenter);
    sp.center.x += x;
    sp.center.z += z;
    // Necesitamos tambien actualizar el CILINDRO
}

void CLPhysics::PositionSphFrontIntoTransf(CTransformable &tr, CBoundingSphere &sp) const {
    sp.center.x = tr.position.x;
    sp.center.z = tr.position.z;
    float x = -cos(Utils::DegToRad(tr.rotation.y)) * (sp.disCenter);
    float z = sin(Utils::DegToRad(tr.rotation.y)) * (sp.disCenter);
    sp.center.x += x;
    sp.center.z += z;
    // Necesitamos tambien actualizar el cilindro
}

void CLPhysics::PositionCilindreIntoSpheres(CBoundingChassis &chassis) const {
    chassis.cilindre->extreme1 = chassis.sphereBehind->center;
    chassis.cilindre->extreme2 = chassis.sphereFront->center;
}

bool CLPhysics::CollisionsCilindreSphere(CTransformable &trCar1, CCar &ccar1, CBoundingChassis &cChaCar1, CTransformable &trCar2, CCar &ccar2, CBoundingChassis &cChaCar2, CExternalForce &cExtForc1, CExternalForce &cExtForc2) {
    //cout << "entramos"<< endl;
    IntersectData inters1 = cChaCar1.cilindre->IntersectSphere(*(cChaCar2.sphereBehind.get()));
    if (inters1.intersects) {
        // CORREGIMOS LA POSICION
        SeparateCilindreSphere(trCar1, inters1.virtualCenter, inters1.virtualRadius, ccar1, trCar2, cChaCar2.sphereBehind->center, cChaCar2.sphereBehind->radius, ccar2);
        ReciveExternalForce(cExtForc1, trCar2, ccar2);
        return true;
    }
    IntersectData inters2 = cChaCar1.cilindre->IntersectSphere(*(cChaCar2.sphereFront.get()));
    if (inters2.intersects) {
        // CORREGIMOS LA POSICION
        SeparateCilindreSphere(trCar1, inters2.virtualCenter, inters2.virtualRadius, ccar1, trCar2, cChaCar2.sphereFront->center, cChaCar2.sphereFront->radius, ccar2);
        
        ReciveExternalForce(cExtForc1, trCar2, ccar2);
        return true;
    }
    IntersectData inters3 = cChaCar2.cilindre->IntersectSphere(*(cChaCar1.sphereBehind.get()));
    if (inters3.intersects) {
        // CORREGIMOS LA POSICION
        SeparateCilindreSphere(trCar2, inters3.virtualCenter, inters3.virtualRadius, ccar2, trCar1, cChaCar1.sphereBehind->center, cChaCar1.sphereBehind->radius, ccar1);
        
        
        ReciveExternalForce(cExtForc2, trCar1, ccar1);
        return true;
    }
    IntersectData inters4 = cChaCar2.cilindre->IntersectSphere(*(cChaCar1.sphereFront.get()));
    if (inters4.intersects) {
        // CORREGIMOS LA POSICION
        SeparateCilindreSphere(trCar2, inters4.virtualCenter, inters4.virtualRadius, ccar2, trCar1, cChaCar1.sphereFront->center, cChaCar1.sphereFront->radius, ccar1);

        ReciveExternalForce(cExtForc2, trCar1, ccar1);
        return true;
    }
    return false;
}

bool CLPhysics::CollisionsSpherePlane(CTransformable &trCar, CBoundingChassis &chaCar, CCar &ccar, bool mainCar, CBoundingPlane &plane) {
    CBoundingSphere *spBehindCar = chaCar.sphereBehind.get();
    bool intersect = false;
    //PositionSphBehindIntoTransf(trCar, *spBehindCar);
    intersect = HandleCollisions(trCar, *spBehindCar, ccar, false, plane);
    if (!intersect) {
        CBoundingSphere *spFrontCar = chaCar.sphereFront.get();
        //PositionSphFrontIntoTransf(trCar, *spFrontCar);
        intersect = HandleCollisions(trCar, *spFrontCar, ccar, false, plane);
    } else {
        intersect = true;
    }
    return intersect;
}

// TODO: Relamente no deberia de ser /2 debera de depender de la fuerza con la que colisionan.... sino puede llegar a temblar, o desplazar
void CLPhysics::SeparateSpheres(CTransformable &trCar1, CBoundingSphere &spCar1, CCar &ccarCar1,
                                CTransformable &trCar2, CBoundingSphere &spCar2, CCar &ccarCar2) const {
    vec3 direction = spCar2.center - spCar1.center;  // te da la dirección al otro bounding en x, y, z, es decir, si tenemos 200, 10, 30, significa que estamos a 200 de distancia en x, a 10 en y y a 30 en z
    float centerDistance = glm::length(direction);
    float distanceCollided = abs(spCar1.radius + spCar2.radius - centerDistance);

    vec3 nuevaDirectionCar1 = -normalize(direction);
    vec3 nuevaDirectionCar2 = normalize(direction);

    //auto totalSpeed = ccarCar1.speed + ccarCar2.speed;

//    if(totalSpeed != 0){
//        auto percCar1 = (100*ccarCar1.speed) / totalSpeed;
//        auto disCorrectCar1 = (distanceCollided*percCar1) / 100;
//        trCar1.position.x += nuevaDirectionCar1.x * (disCorrectCar1 / 2);
//        trCar1.position.z += nuevaDirectionCar1.z * (disCorrectCar1 / 2);
//
//        auto percCar2 = (100*ccarCar2.speed) / totalSpeed;
//        auto disCorrectCar2 = (distanceCollided*percCar2) / 100;
//        trCar2.position.x += nuevaDirectionCar2.x * (disCorrectCar2 / 2);
//        trCar2.position.z += nuevaDirectionCar2.z * (disCorrectCar2 / 2);
//    }else{
        trCar1.position.x += nuevaDirectionCar1.x * (distanceCollided / 2);
        trCar1.position.z += nuevaDirectionCar1.z * (distanceCollided / 2);
        trCar2.position.x += nuevaDirectionCar2.x * (distanceCollided / 2);
        trCar2.position.z += nuevaDirectionCar2.z * (distanceCollided / 2);
//    }

}

void CLPhysics::SeparateCilindreSphere(CTransformable &trCar1, glm::vec3 &cenCar1, const float &radCar1, CCar &ccarCar1, CTransformable &trCar2, glm::vec3 &cenCar2, const float &radCar2, CCar &ccarCar2) const {
    vec3 direction = cenCar2 - cenCar1;  // te da la dirección al otro bounding en x, y, z, es decir, si tenemos 200, 10, 30, significa que estamos a 200 de distancia en x, a 10 en y y a 30 en z
    float centerDistance = glm::length(direction);
    float distanceCollided = abs(radCar1 + radCar2 - centerDistance);

    vec3 nuevaDirectionCar1 = -normalize(direction);
    vec3 nuevaDirectionCar2 = normalize(direction);

    //auto totalSpeed = ccarCar1.speed + ccarCar2.speed;

//    if(totalSpeed != 0){
//        auto percCar1 = (100*ccarCar1.speed) / totalSpeed;
//        auto disCorrectCar1 = (distanceCollided*percCar1) / 100;
//        trCar1.position.x += nuevaDirectionCar1.x * (disCorrectCar1 / 2);
//        trCar1.position.z += nuevaDirectionCar1.z * (disCorrectCar1 / 2);
//
//        auto percCar2 = (100*ccarCar2.speed) / totalSpeed;
//        auto disCorrectCar2 = (distanceCollided*percCar2) / 100;
//        trCar2.position.x += nuevaDirectionCar2.x * (disCorrectCar2 / 2);
//        trCar2.position.z += nuevaDirectionCar2.z * (disCorrectCar2 / 2);
//    }else{
        trCar1.position.x += nuevaDirectionCar1.x * (distanceCollided / 2);
        trCar1.position.z += nuevaDirectionCar1.z * (distanceCollided / 2);
        trCar2.position.x += nuevaDirectionCar2.x * (distanceCollided / 2);
        trCar2.position.z += nuevaDirectionCar2.z * (distanceCollided / 2);
//    }
}

// HandleCollisions entre ESFERA de un COCHE y un PLANO
bool CLPhysics::HandleCollisions(CTransformable &trCar, CBoundingSphere &spCar, CCar &ccarCar, bool mainCar, CBoundingPlane &plane) {
    IntersectData intersData = plane.IntersectSphere(spCar);
    if (intersData.intersects) {
        SeparateSphereFromPlane(intersData, trCar, spCar, plane);

        vec3 vecDirCar = CalculateVecDirCar(trCar);
        double angle2V = Angle2Vectors(vecDirCar, plane.normalizedNormal);
        if (angle2V > 130.0) {  // no debemos reflejar del todo
            if (ccarCar.speed >= ccarCar.maxSpeed * 0.3) {
                ccarCar.speed = ccarCar.maxSpeed * 0.3;
            }
        } else {
            if (ccarCar.speed >= ccarCar.maxSpeed * 0.8) {
                ccarCar.speed = ccarCar.maxSpeed * 0.8;
            }
        }
        return true;
    }
    return false;
}

// HandleCollisions entre ESFERA de un COCHE y un OBB
bool CLPhysics::HandleCollisions(CTransformable &trCar, CBoundingSphere &spCar, CCar &ccarCar, bool mainCar, CBoundingOBB &obb) {
    IntersectData intersData = obb.IntersectSphere(spCar);
    if (intersData.intersects) {
        SeparateSphereFromPlane(intersData, trCar, spCar, *obb.planes[intersData.posEntity]);

        // dependiendo de como colisionemos con el plano sera un tipo de colision u otro.
        vec3 vecDirCar = CalculateVecDirCar(trCar);
        double angle2V = Angle2Vectors(vecDirCar, obb.planes[intersData.posEntity]->normalizedNormal);
        if (angle2V > 130.0) {  // no debemos reflejar del todo
            if (ccarCar.speed >= ccarCar.maxSpeed * 0.3) {
                ccarCar.speed = ccarCar.maxSpeed * 0.3;
            }
        } else {
            if (ccarCar.speed >= ccarCar.maxSpeed * 0.8) {
                ccarCar.speed = ccarCar.maxSpeed * 0.8;
            }
        }
        return true;
    }
    return false;
}

// TODO: no hay colisiones entre CILINDRO-PLANO
void CLPhysics::CollisionsSphereOBB(CTransformable &trCar, CBoundingChassis &chaCar, CCar &ccar, bool mainCar, CBoundingOBB &obb) {
    CBoundingSphere *spBehindCar = chaCar.sphereBehind.get();
    bool intersect = false;
    //PositionSphBehindIntoTransf(trCar, *spBehindCar);
    intersect = HandleCollisions(trCar, *spBehindCar, ccar, false, obb);
    if (!intersect) {
        CBoundingSphere *spFrontCar = chaCar.sphereFront.get();
        //PositionSphFrontIntoTransf(trCar, *spFrontCar);
        HandleCollisions(trCar, *spFrontCar, ccar, false, obb);
    }
}

vec3 CLPhysics::CalculateVecDirCar(CTransformable &cTransformable) const {
    float angleRotation = (cTransformable.rotation.y * M_PI) / 180.0;
    float nextPosX = cTransformable.position.x - cos(angleRotation) * 1;
    float nexPosZ = cTransformable.position.z + sin(angleRotation) * 1;

    return vec3(nextPosX - cTransformable.position.x, 0, nexPosZ - cTransformable.position.z);
}

void CLPhysics::SeparateSphereFromPlane(IntersectData &intersData, CTransformable &trCar1, CBoundingSphere &spCar1, CBoundingPlane &plane) const {
    //cout << "NO SEPARO" << endl;
    vec3 direction = spCar1.center - plane.normal;  // te da la dirección al otro bounding en x, y, z, es decir, si tenemos 200, 10, 30, significa que estamos a 200 de distancia en x, a 10 en y y a 30 en z
    vec3 nuevaDirectionCar1 = -normalize(direction);
    float correctedDistance = intersData.GetDistance() + 0.05;
    trCar1.position.x += nuevaDirectionCar1.x * correctedDistance;
    trCar1.position.z += nuevaDirectionCar1.z * correctedDistance;
}

/**
 * Recibe los componentes de los dos coches con los que se comprobará colisión
 * El bool mainCar define si los componentes del coche1 son los del coche principal o no
 */
bool CLPhysics::HandleCollisions(CTransformable &trCar1, CBoundingSphere &spCar1, CCar &ccarCar1, bool mainCar, CTransformable &trCar2, CBoundingSphere &spCar2, CCar &ccarCar2, CExternalForce &cExtForc1, CExternalForce &cExtForc2) {
    // posicionamos la esfera en la misma posición que el coche pero teniendo en cuenta el offset

    PositionSphereIntoTransformable(trCar1, spCar1);
    PositionSphereIntoTransformable(trCar2, spCar2);
    IntersectData intersData = spCar1.IntersectSphere(spCar2);
    if (intersData.intersects) {
        SeparateSpheres(trCar1, spCar1, ccarCar1, trCar2, spCar2, ccarCar2);
        PositionSphereIntoTransformable(trCar1, spCar1);
        PositionSphereIntoTransformable(trCar2, spCar2);

        // ADEMAS DEBEMOS REDUCIRLES LAS VELOCIDADES POR CHOCAR
        impactCarCar(ccarCar1, ccarCar2);

        return true;  // los coches han colisionado -- lo utilizamos para robar el totem si llevamos el nitro
    }
    return false;  // los coches no han colisionado
}

bool CLPhysics::CollisionsSphereSphere(CTransformable &trCar1, CBoundingChassis &chaCar1, CCar &ccar1, bool mainCar, CTransformable &trCar2, CBoundingChassis &chaCar2, CCar &ccar2, CExternalForce &cExtForc1, CExternalForce &cExtForc2) {
    CBoundingSphere *spBehindCar1 = chaCar1.sphereBehind.get();
    CBoundingSphere *spFrontCar1 = chaCar1.sphereFront.get();

    CBoundingSphere *spBehindCar2 = chaCar2.sphereBehind.get();
    CBoundingSphere *spFrontCar2 = chaCar2.sphereFront.get();

    bool intersect = false;
    intersect = CollisionsFrontFront(trCar1, *spFrontCar1, ccar1, mainCar, trCar2, *spFrontCar2, ccar2, cExtForc1, cExtForc2);
    if (!intersect) {
        intersect = CollisionsFrontBehind(trCar1, *spFrontCar1, ccar1, mainCar, trCar2, *spBehindCar2, ccar2, cExtForc1, cExtForc2);
        if (!intersect) {
            intersect = CollisionsBehindFront(trCar1, *spBehindCar1, ccar1, mainCar, trCar2, *spFrontCar2, ccar2, cExtForc1, cExtForc2);
            if (!intersect) {
                intersect = CollisionsBehindBehind(trCar1, *spBehindCar1, ccar1, mainCar, trCar2, *spBehindCar2, ccar2, cExtForc1, cExtForc2);
            }
        }
    }
    return intersect;
}

bool CLPhysics::CollisionsBehindBehind(CTransformable &trCar1, CBoundingSphere &spCar1, CCar &ccarCar1, bool mainCar, CTransformable &trCar2, CBoundingSphere &spCar2, CCar &ccarCar2, CExternalForce &cExtForc1, CExternalForce &cExtForc2) {

    IntersectData intersData = spCar1.IntersectSphere(spCar2);
    if (intersData.intersects) {
        //SonarChoque(mainCar);
        SeparateSpheres(trCar1, spCar1, ccarCar1, trCar2, spCar2, ccarCar2);

        impactCarCar(ccarCar1, ccarCar2);

        return true;
    }
    return false;
}

bool CLPhysics::CollisionsBehindFront(CTransformable &trCar1, CBoundingSphere &spCar1, CCar &ccarCar1, bool mainCar, CTransformable &trCar2, CBoundingSphere &spCar2, CCar &ccarCar2, CExternalForce &cExtForc1, CExternalForce &cExtForc2) {

    IntersectData intersData = spCar1.IntersectSphere(spCar2);
    if (intersData.intersects) {
        SeparateSpheres(trCar1, spCar1, ccarCar1, trCar2, spCar2, ccarCar2);


        ReciveExternalForce(cExtForc1, trCar2, ccarCar2);
        impactCarCar(ccarCar1, ccarCar2);

        return true;  // los coches han colisionado -- lo utilizamos para robar el totem si llevamos el nitro
    }
    return false;  // los coches no han colisionado
}

bool CLPhysics::CollisionsFrontBehind(CTransformable &trCar1, CBoundingSphere &spCar1, CCar &ccarCar1, bool mainCar, CTransformable &trCar2, CBoundingSphere &spCar2, CCar &ccarCar2, CExternalForce &cExtForc1, CExternalForce &cExtForc2) {

    IntersectData intersData = spCar1.IntersectSphere(spCar2);
    if (intersData.intersects) {
        SeparateSpheres(trCar1, spCar1, ccarCar1, trCar2, spCar2, ccarCar2);


        ReciveExternalForce(cExtForc2, trCar1, ccarCar1);
        impactCarCar(ccarCar1, ccarCar2);

        return true;  // los coches han colisionado -- lo utilizamos para robar el totem si llevamos el nitro
    }
    return false;  // los coches no han colisionado
}

bool CLPhysics::CollisionsFrontFront(CTransformable &trCar1, CBoundingSphere &spCar1, CCar &ccarCar1, bool mainCar, CTransformable &trCar2, CBoundingSphere &spCar2, CCar &ccarCar2, CExternalForce &cExtForc1, CExternalForce &cExtForc2) {
    IntersectData intersData = spCar1.IntersectSphere(spCar2);
    if (intersData.intersects) {
        //SonarChoque(mainCar);
        SeparateSpheres(trCar1, spCar1, ccarCar1, trCar2, spCar2, ccarCar2);

        //cout << "EL COCHE 1 speed: " << ccarCar1.speed << endl;
        //cout << "EL COCHE 2 speed: " << ccarCar2.speed << endl; 

        ReciveExternalForce(cExtForc1, trCar2, ccarCar2);
        ReciveExternalForce(cExtForc2, trCar1, ccarCar1);
        //cout << "cExtForcee del PLAYEEEER: " << cExtForc1.force << endl;
        //cout << "cExtForcee del OTROOO: " << cExtForc2.force << endl;
        impactCarCar(ccarCar1, ccarCar2);

        return true;  // los coches han colisionado -- lo utilizamos para robar el totem si llevamos el nitro
    }
    return false;  // los coches no han colisionado
}
void CLPhysics::ReciveExternalForce(CExternalForce &cExtForc1, CTransformable &trCarOther, CCar &ccarOther) {
    glm::vec3 vecDirCar2 = CalculateVecDirCar(trCarOther);
    if (ccarOther.speed > ccarOther.maxSpeed * 0.1) {
        cExtForc1.dirExternalForce = vecDirCar2;
        //cout << "HOLAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA: " << ccarOther.speed << endl;
        cExtForc1.force = ccarOther.speed  + ccarOther.weight;
    }

}


void CLPhysics::impactCarCar(CCar &ccar1, CCar &ccar2) const {
    if (ccar1.speed >= ccar1.maxSpeed * 0.5) {
        ccar1.speed = ccar1.maxSpeed * 0.5;
    }
    if (ccar2.speed >= ccar2.maxSpeed * 0.5) {
        ccar2.speed = ccar2.maxSpeed * 0.5;
    }
}



IntersectData CLPhysics::HandleCollisionsRayWithSpheres(CTransformable &trCar1, CTransformable &trCar2, CBoundingSphere &spCar2, const glm::vec3 &normalRay) {
    PositionSphereIntoTransformable(trCar2, spCar2);
    IntersectData intersData = spCar2.IntersectRay(trCar1, normalRay);
    return intersData;
}

IntersectData CLPhysics::HandleCollisionsRayWithPlane(CTransformable &trRayOrigin, glm::vec3 &rayNormalNormalized, CBoundingPlane &planeObject) {
    glm::vec3 positionRayOrigin(trRayOrigin.position.x, trRayOrigin.position.y, trRayOrigin.position.z);
    IntersectData intersData = planeObject.IntersectRay2(positionRayOrigin, rayNormalNormalized);
    return intersData;
}

IntersectData CLPhysics::HandleCollisionsRayWithOBB(CTransformable &trRayOrigin, glm::vec3 &rayNormalNormalized, CBoundingOBB &OBBObject) {
    glm::vec3 positionRayOrigin(trRayOrigin.position.x, trRayOrigin.position.y, trRayOrigin.position.z);
    
    IntersectData intersData = OBBObject.IntersectRay2(positionRayOrigin, rayNormalNormalized);
    if (intersData.intersects) {
        return intersData;
    }

    return IntersectData(false, vec3(0,0,0));
}

double CLPhysics::Angle2Vectors(const vec3 &a, const vec3 &b) const {
    vec3 aN = glm::normalize(a);
    vec3 bN = glm::normalize(b);

    double dot = glm::dot(aN, bN);
    dot = (dot < -1.0 ? -1.0 : (dot > 1.0 ? 1.0 : dot));

    double angleRad = acos(dot);
    // grados = radianes*(180/PI_)
    return angleRad * (180 / M_PI);
}

void CLPhysics::IntersectCarsTotem(ManCar &manCars, ManTotem &manTotem) {
    bool collision = false;
    for (long unsigned int i = 0; i < manCars.GetEntities().size() && !collision; i++) {
        const auto &currentCar = manCars.GetEntities()[i];
        auto cChassisCar = static_cast<CBoundingChassis *>(currentCar.get()->GetComponent(CompType::CompBoundingChassis).get());
        auto cTotem = static_cast<CTotem *>(manTotem.GetEntities()[0]->GetComponent(CompType::TotemComp).get());

        if (cTotem->active) {
            CBoundingSphere *cSphereTotem = static_cast<CBoundingSphere *>(manTotem.GetEntities()[0]->GetComponent(CompType::CompBoundingSphere).get());
            auto intersect = cChassisCar->cilindre->IntersectSphere(*cSphereTotem);
            if (!intersect.intersects)
                intersect = cChassisCar->sphereBehind->IntersectSphere(*cSphereTotem);
            if (!intersect.intersects)
                intersect = cChassisCar->sphereFront->IntersectSphere(*cSphereTotem);
            if (intersect.intersects) {  //TRUE
                collision = true;
                // debemos coger el TOTEM
                //cout << "colisionamos con el totem " << endl;
                shared_ptr<DataMap> dataCollisionTotem = make_shared<DataMap>();
                (*dataCollisionTotem)[TOTEM] = manTotem.GetEntities()[0];  // nos guardamos el puntero para eliminar el powerUp
                (*dataCollisionTotem)[ACTUAL_CAR] = currentCar.get();
                EventManager::GetInstance().AddEventMulti(Event{EventType::COLLISION_PLAYER_TOTEM, dataCollisionTotem});
            }
        }
    }
}

void CLPhysics::IntersectCarsBoxPowerUp(ManCar &manCars, ManBoxPowerUp &manBoxPowerUp) {
    // IntersectData inters1 = cChaCar1.cilindre->IntersectSphere(*(cChaCar2.sphereBehind.get()));
    for (const auto &currentBoxPowerUp : manBoxPowerUp.GetEntities()) {
        auto cBoxPowerUp = static_cast<CBoxPowerUp *>(currentBoxPowerUp->GetComponent(CompType::BoxPowerUpComp).get());
        CBoundingSphere *cSpherePU = static_cast<CBoundingSphere *>(currentBoxPowerUp->GetComponent(CompType::CompBoundingSphere).get());

        bool collision = false;
        for (long unsigned int i = 0; i < manCars.GetEntities().size() && !collision; i++) {
            const auto &currentCar = manCars.GetEntities()[i];
            auto cPowerUpCar = static_cast<CPowerUp *>(currentCar.get()->GetComponent(CompType::PowerUpComp).get());
            auto cChassisCar = static_cast<CBoundingChassis *>(currentCar.get()->GetComponent(CompType::CompBoundingChassis).get());

            // Vemos si efectivamente esta activo o no, para poder cogerlo
            if (cBoxPowerUp->active == true) {
                // primero vemos si colisionan
                auto intersect = cChassisCar->cilindre->IntersectSphere(*cSpherePU);
                if (!intersect.intersects)
                    intersect = cChassisCar->sphereBehind->IntersectSphere(*cSpherePU);
                if (!intersect.intersects)
                    intersect = cChassisCar->sphereFront->IntersectSphere(*cSpherePU);
                if (intersect.intersects) {  //TRUE
                    collision = true;
                    if (cPowerUpCar->typePowerUp == typeCPowerUp::None) {
                        shared_ptr<DataMap> dataCollisonCarBoxPowerUp = make_shared<DataMap>();  // Mejor definirlo en el .h
                        (*dataCollisonCarBoxPowerUp)[BOX_POWER_UP_COMPONENT] = cBoxPowerUp;
                        (*dataCollisonCarBoxPowerUp)[ACTUAL_BOX] = currentBoxPowerUp;
                        (*dataCollisonCarBoxPowerUp)[ACTUAL_CAR] = currentCar.get();
                        // Lanzaremos este evento cuando colisionemos con una caja y no tengamos ya PowerUp
                        EventManager::GetInstance().AddEventMulti(Event{EventType::CATCH_AI_BOX_POWERUP, dataCollisonCarBoxPowerUp});
                    } else {
                        shared_ptr<DataMap> dataCollisonCarBoxPowerUp = make_shared<DataMap>();  // Mejor definirlo en el .h
                        (*dataCollisonCarBoxPowerUp)[BOX_POWER_UP_COMPONENT] = cBoxPowerUp;
                        (*dataCollisonCarBoxPowerUp)[ACTUAL_BOX] = currentBoxPowerUp;
                        // Lanzaremos este evento cuando colisionemos con una caja y tengamos ya PowerUp
                        EventManager::GetInstance().AddEventMulti(Event{EventType::CATCH_BOX_WITH_POWERUP, dataCollisonCarBoxPowerUp});
                    }
                }
            }
        }
    }
}

void CLPhysics::IntersectsCarsPowerUps(ManCar &manCars, ManPowerUp &manPowerUps, ManNavMesh *manNavMesh) {
    for (const auto &currentPU : manPowerUps.GetEntities()) {
        CBoundingSphere *cSpherePU = static_cast<CBoundingSphere *>(currentPU.get()->GetComponent(CompType::CompBoundingSphere).get());

        bool collision = false;
        for (long unsigned int i = 0; i < manCars.GetEntities().size() && !collision; i++) {
            const auto &currentCar = manCars.GetEntities()[i];
            auto cChassisCar = static_cast<CBoundingChassis *>(currentCar.get()->GetComponent(CompType::CompBoundingChassis).get());

            auto intersect = cChassisCar->cilindre->IntersectSphere(*cSpherePU);
            if (!intersect.intersects)
                intersect = cChassisCar->sphereBehind->IntersectSphere(*cSpherePU);
            if (!intersect.intersects)
                intersect = cChassisCar->sphereFront->IntersectSphere(*cSpherePU);
            if (intersect.intersects) {  //TRUE
                collision = true;
                // si estamos en online, avisamos al server de que un PU ha chocado con un coche,
                // pero no lanzamos el choque en sí porque lo gestionará el server
                if (systemOnline != nullptr) {
                    CIDOnline *cidOnline = static_cast<CIDOnline *>(currentPU->GetComponent(CompType::CIDOnlineComp).get());
                    // si no se había colisionado ya con este PU..
                    if (!cidOnline->collided) {
                        cidOnline->collided = true;
                        COnline *carOnlineComp = static_cast<COnline *>(currentCar->GetComponent(CompType::OnlineComp).get());
                        // CTransformable *transforPU = static_cast<CTransformable *>(currentPU->GetComponent(CompType::TransformableComp).get());
                        // transforPU->position.y -= 500;  // esto igual se podría eliminar directamente el PU en vez de ocultarlo bajo el suelo
                        systemOnline->SendCrashPUCar(cidOnline->idOnline, carOnlineComp->idClient);
                        //cout << "Hemos chocado con el PU-Car con el pu[" << cidOnline->idOnline << "] car[" << carOnlineComp->idClient << "] y lo vamos a enviar al servidor" << endl;
                    }

                    // si estamos en el single, lanzamos directamente el choque
                } else {
                    HandleCollisionPUWithCar(static_cast<PowerUp *>(currentPU.get()), currentCar.get());
                }
            }
        }
    }
}

void CLPhysics::HandleCollisionPUWithCar(PowerUp *powerUp, Entity *car) {
    // ponemos a true el componente DeleteEntity, para eliminarlo con seguridad beibeee
    auto cRemovableObj = static_cast<CRemovableObject *>(powerUp->GetComponent(CompType::RemovableObjectComp).get());
    cRemovableObj->destroy = true;

    // comprobamos si el coche tenia escudo y el totem.. ya que debe de soltarlo
    auto cShield = static_cast<CShield *>(car->GetComponent(CompType::ShieldComp).get());
    if (cShield->activePowerUp == false) {  // TRUE
        //cout << "Le han danyado y NO tiene escudo" << endl;
        auto cHurt = static_cast<CHurt *>(car->GetComponent(CompType::HurtComp).get());
        if(!cHurt->hurt) {
            //cout << "Le han danyado y NO estaba danyado" << endl;
            // debemos hacer danyo al jugador
            shared_ptr<DataMap> dataCollisonCarPowerUp = make_shared<DataMap>();
            (*dataCollisonCarPowerUp)[ACTUAL_CAR] = car;  // nos guardamos el puntero al coche
            EventManager::GetInstance().AddEventMulti(Event{EventType::COLLISION_CAR_POWERUP, dataCollisonCarPowerUp});

            if (static_cast<CTotem *>(car->GetComponent(CompType::TotemComp).get())->active) {
                //cout << "Le han danyado y SÍ tenían totem" << endl;
                auto dataTransformableCar = static_cast<CTransformable *>(car->GetComponent(CompType::TransformableComp).get());
                shared_ptr<DataMap> dataTransfCar = make_shared<DataMap>();
                (*dataTransfCar)[CAR_TRANSFORMABLE] = dataTransformableCar;
                (*dataTransfCar)[ACTUAL_CAR] = car;
                EventManager::GetInstance().AddEventMulti(Event{EventType::DROP_TOTEM, dataTransfCar});
            }
        }
    } else {
        auto cId = static_cast<CId*>(car->GetComponent(CompType::IdComp).get());
        cShield->deactivePowerUp();  // desactivamos el escudo
        shared_ptr<DataMap> data = make_shared<DataMap>();
        (*data)[ID] = cId->id;
        (*data)[TRUEFALSE] = false;
        EventManager::GetInstance().AddEventMulti(Event{EventType::UPDATE_SHIELD_VISIBILITY, data});

        // Sonido romper escudo
        auto cPos = static_cast<CTransformable*>(car->GetComponent(CompType::TransformableComp).get());
        if ( cId && cPos) {
            shared_ptr<DataMap> dataSoundShild = make_shared<DataMap>();
            (*dataSoundShild)[ID] = cId->id;
            (*dataSoundShild)[VEC3_POS] = cPos->position;
            EventManager::GetInstance().AddEventMulti(Event{EventType::NO_SHIELD, dataSoundShild});
        }
    }
}

void CLPhysics::IntersectPowerUpWalls(ManPowerUp &manPowerUp, ManBoundingWall &manWalls, ManBoundingOBB &manOBB) {
    for (const auto &currentPU : manPowerUp.GetEntities()) {
        CBoundingSphere *cSpherePU = static_cast<CBoundingSphere *>(currentPU->GetComponent(CompType::CompBoundingSphere).get());

        bool collision = false;
        // COMPROBAMOS LOS PLANOS NORMALES
        for (long unsigned int i = 0; i < manWalls.GetEntities().size() && !collision; i++) {
            const auto &currentWall = manWalls.GetEntities()[i];
            CBoundingPlane *plane = static_cast<CBoundingPlane *>(currentWall->GetComponent(CompType::CompBoundingPlane).get());

            IntersectData intersect = plane->IntersectSphere(*cSpherePU);
            if (intersect.intersects) {
                collision = true;
                // el powerUp ha tocado una pared debemos eliminarlo
                // si estamos en online, avisamos al server
                if (systemOnline != nullptr) {
                    CIDOnline *cidOnline = static_cast<CIDOnline *>(currentPU->GetComponent(CompType::CIDOnlineComp).get());
                    // si no se había colisionado ya con este PU..
                    if (!cidOnline->collided) {
                        cidOnline->collided = true;
                        systemOnline->SendCrashPUWall(cidOnline->idOnline);
                    }

                    // si estamos en el single, eliminamos directamente el PU
                } else {
                    auto cRemovableObj = static_cast<CRemovableObject *>(currentPU.get()->GetComponent(CompType::RemovableObjectComp).get());
                    cRemovableObj->destroy = true;
                }
            }
        }
        // COMPROBAMOS LOS OBB
        for (long unsigned int i = 0; i < manOBB.GetEntities().size() && !collision; i++) {
            const auto &currentOBB = manOBB.GetEntities()[i];
            CBoundingOBB *cOBBactual = static_cast<CBoundingOBB *>(currentOBB->GetComponent(CompType::CompBoundingOBB).get());

            IntersectData intersect = cOBBactual->IntersectSphere(*cSpherePU);
            if (intersect.intersects) {
                collision = true;
                if (systemOnline != nullptr) {
                    CIDOnline *cidOnline = static_cast<CIDOnline *>(currentPU->GetComponent(CompType::CIDOnlineComp).get());
                    // si no se había colisionado ya con este PU..
                    if (!cidOnline->collided) {
                        cidOnline->collided = true;
                        systemOnline->SendCrashPUWall(cidOnline->idOnline);
                    }
                } else {
                    auto cRemovableObj = static_cast<CRemovableObject *>(currentPU.get()->GetComponent(CompType::RemovableObjectComp).get());
                    cRemovableObj->destroy = true;
                }
            }
        }
    }
}



void CLPhysics::IntersectTotemWalls(ManTotem &manTotem, ManBoundingWall &manWalls, ManBoundingOBB &manOBB) {
    const auto &totem = manTotem.GetEntities()[0];
    CBoundingSphere *cBSTotem = static_cast<CBoundingSphere *>(totem->GetComponent(CompType::CompBoundingSphere).get());
    auto cTotem = static_cast<CTotem*>(totem->GetComponent(CompType::TotemComp).get());
    bool collision = false;
    // COMPROBAMOS LOS PLANOS NORMALES
    for (long unsigned int i = 0; i < manWalls.GetEntities().size() && !collision; i++) {
        const auto &currentWall = manWalls.GetEntities()[i];
        CBoundingPlane *plane = static_cast<CBoundingPlane *>(currentWall->GetComponent(CompType::CompBoundingPlane).get());
        IntersectData intersect = plane->IntersectSphere(*cBSTotem);
        if (intersect.intersects) {
            // COLISION CON WALL -> speed a 0, beibe
            collision = true;
            cTotem->speed = cTotem->MaxSpeed;
        }
    }
    // COMPROBAMOS LOS OBB
    for (long unsigned int i = 0; i < manOBB.GetEntities().size() && !collision; i++) {
        const auto &currentOBB = manOBB.GetEntities()[i];
        CBoundingOBB *cOBBactual = static_cast<CBoundingOBB *>(currentOBB->GetComponent(CompType::CompBoundingOBB).get());
        IntersectData intersect = cOBBactual->IntersectSphere(*cBSTotem);
        if (intersect.intersects) {
            collision = true;
            cTotem->speed = cTotem->MaxSpeed;
        }
    }
}

void CLPhysics::IntersectCameraWalls(Camera *cam, Car* car, ManBoundingWall &manWalls, ManBoundingOBB &manOBB){
    //auto cam = manCam.getCamera();
    auto cTransfCar = static_cast<CTransformable*>(car->GetComponent(CompType::TransformableComp).get());
    auto cTransfCam = static_cast<CTransformable*>(cam->GetComponent(CompType::TransformableComp).get());
    CBoundingSphere *cBSCam = static_cast<CBoundingSphere *>(cam->GetComponent(CompType::CompBoundingSphere).get());

    cBSCam->center = cTransfCam->position;
    auto cCamera = static_cast<CCamera*>(cam->GetComponent(CompType::CameraComp).get());
    bool collision = false;
        auto vecNCam = glm::normalize(cTransfCam->position - cTransfCar->position);
    // COMPROBAMOS LOS PLANOS NORMALES
    for (long unsigned int i = 0; i < manWalls.GetEntities().size() && !collision; i++) {
        const auto &currentWall = manWalls.GetEntities()[i];
        CBoundingPlane *plane = static_cast<CBoundingPlane *>(currentWall->GetComponent(CompType::CompBoundingPlane).get());
        IntersectData intersData = plane->IntersectRay2(cTransfCar->position, vecNCam);
        if (intersData.intersects) {
            if(intersData.distance < cCamera->actualDistance){
                collision = true;
                cCamera->collisionDistance = cCamera->actualDistance - intersData.distance;
            }
        }
    }
    // COMPROBAMOS LOS OBB
    for (long unsigned int i = 0; i < manOBB.GetEntities().size() && !collision; i++) {
        const auto &currentOBB = manOBB.GetEntities()[i];
        CBoundingOBB *cOBBcurrent = static_cast<CBoundingOBB *>(currentOBB->GetComponent(CompType::CompBoundingOBB).get());
        auto planesOBB = cOBBcurrent->GetPlanesOBB();
        for( long unsigned int i = 0; i < planesOBB.size() && !collision; i++){
            const auto &currentPlaneOBB = planesOBB[i];
            IntersectData intersData = currentPlaneOBB.get()->IntersectRay2(cTransfCar->position, vecNCam);
            if (intersData.intersects) {
                if(intersData.distance < cCamera->actualDistance){
                    collision = true;
                    if(intersData.distance < 3) intersData.distance = 3;
                    cCamera->collisionDistance = cCamera->actualDistance - intersData.distance;
                }
            }
        }
    }
    if(!collision){
        cCamera->collisionDistance = 0.0;   
    }
}







void CLPhysics::RunTests() {
    CBoundingSphere sp1(vec3(0.f, 0.f, 0.f), 1.f);
    CBoundingSphere sp2(vec3(0.f, 3.f, 0.f), 1.f);
    CBoundingSphere sp3(vec3(0.f, 0.f, 2.f), 1.f);
    CBoundingSphere sp4(vec3(1.f, 0.f, 0.f), 1.f);

    IntersectData sp1sp2 = sp1.IntersectSphere(sp2);
    IntersectData sp1sp3 = sp1.IntersectSphere(sp3);
    IntersectData sp1sp4 = sp1.IntersectSphere(sp4);

    assert(sp1sp2.intersects == false);
    assert(sp1sp2.GetDistance() == 1);

    assert(sp1sp3.intersects == false);
    assert(sp1sp3.GetDistance() == 0);

    assert(sp1sp4.intersects == true);
    assert(sp1sp4.GetDistance() == -1);
    //cout << "sp1 intersects sp2: " << sp1sp2.intersects << ", Distance: " << sp1sp2.GetDistance() << endl;
    //cout << "sp1 intersects sp3: " << sp1sp3.intersects << ", Distance: " << sp1sp3.GetDistance() << endl;
    //cout << "sp1 intersects sp4: " << sp1sp4.intersects << ", Distance: " << sp1sp4.GetDistance() << endl
    //     << endl;

    CAABoundingBox aabb1(vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f));
    CAABoundingBox aabb2(vec3(1.f, 1.f, 1.f), vec3(2.f, 2.f, 2.f));
    CAABoundingBox aabb3(vec3(1.f, 0.f, 0.f), vec3(2.f, 1.f, 1.f));
    CAABoundingBox aabb4(vec3(0.f, 0.f, -2.f), vec3(1.f, 1.f, -1.f));
    CAABoundingBox aabb5(vec3(0.f, 0.5f, 0.f), vec3(1.f, 1.5f, 1.f));

    IntersectData aabb1IDinters2 = aabb1.IntersectCAABoundingBox(aabb2);
    IntersectData aabb1IDinters3 = aabb1.IntersectCAABoundingBox(aabb3);
    IntersectData aabb1IDinters4 = aabb1.IntersectCAABoundingBox(aabb4);
    IntersectData aabb1IDinters5 = aabb1.IntersectCAABoundingBox(aabb5);

    assert(aabb1IDinters2.intersects == false);
    assert(aabb1IDinters2.GetDistance() == 0);

    assert(aabb1IDinters3.intersects == false);
    assert(aabb1IDinters3.GetDistance() == 0);

    assert(aabb1IDinters4.intersects == false);
    assert(aabb1IDinters4.GetDistance() == 1);

    assert(aabb1IDinters5.intersects == true);
    assert(aabb1IDinters5.GetDistance() == -0.5);

    //cout << "AABB1 intersects AABB2: " << aabb1IDinters2.intersects << ", Distance: " << aabb1IDinters2.GetDistance() << endl;
    //cout << "AABB1 intersects AABB3: " << aabb1IDinters3.intersects << ", Distance: " << aabb1IDinters3.GetDistance() << endl;
    //cout << "AABB1 intersects AABB4: " << aabb1IDinters4.intersects << ", Distance: " << aabb1IDinters4.GetDistance() << endl;
    //cout << "AABB1 intersects AABB5: " << aabb1IDinters5.intersects << ", Distance: " << aabb1IDinters5.GetDistance() << endl
    //     << endl;
}

void VersionRayoVectores() {
    /*vec3 direccionCar1;
            direccionCar1.x = cos(anguloCar1 * M_PI / 180.0);
            direccionCar1.y = 0;
            direccionCar1.z = sin(anguloCar1 * M_PI / 180.0);
            CRay cray1(spCar1.center, normalize(direccionCar1));

            vec3 direccionCar2;
            direccionCar2.x = cos(anguloCar2 * M_PI / 180.0);
            direccionCar2.y = 0;
            direccionCar2.z = sin(anguloCar2 * M_PI / 180.0);
            CRay cray2(spCar2.center, normalize(direccionCar2));

            float cRay1IntersectsSP2 = cray1.IntersectSphere(spCar2);
            if (cRay1IntersectsSP2 > 0) {
                string msg = MakeString() << "car1 le tira rayo a car2";
                Utils::Cout(msg);

                // al coche 1 lo hacemos retroceder
                cout << "car 1 le tira rayo a car2" << endl;
                ccarCar1.speed = -5 - ccarCar1.speed / 3;

                // al coche 2 lo paramos
                ccarCar2.speed = 0.f;
            }

            float cRay2IntersectsSP1 = cray2.IntersectSphere(spCar1);
            if (cRay2IntersectsSP1 > 0) {
                string msg = MakeString() << "car2 le tira rayo a car1";
                Utils::Cout(msg);
                cout << "car 2 le tira rayo a car1" << endl;

                // al coche 2 lo hacemos retroceder
                ccarCar2.speed = -5 - ccarCar2.speed / 3;

                // al coche 1 lo paramos
                ccarCar1.speed = 0.f;
            }*/
}

// versión suma de vectores + desviación
void VersionSumaVectores() {
    /*vec3 directionCar1 = Utils::GetVectorFromAngle(anguloCar1);
            vec3 directionCar2 = Utils::GetVectorFromAngle(anguloCar2);
            
            vec3 vectorSum = directionCar1 + directionCar2;
            float gradoSalida = Utils::GetAngleFromVector(vectorSum);
            vec3 vectorSalida1 = normalize(vectorSum + (directionCar2 / 3.f));
            float gradoSalida1 = Utils::GetAngleFromVector(vectorSalida1);
            vec3 vectorSalida2 = normalize(vectorSum + (directionCar1 / 3.f));
            float gradoSalida2 = Utils::GetAngleFromVector(vectorSalida2);
            float nuevoAnguloCar1Deg = Utils::GetAngleFromVector(vectorSalida1);
            float nuevoAnguloCar2Deg = Utils::GetAngleFromVector(vectorSalida2);
            
            trCar1.rotation.y = nuevoAnguloCar1Deg;
            trCar2.rotation.y = nuevoAnguloCar2Deg;
            
            // trCar1.rotation.x = vectorSalida1.z;
            // trCar1.rotation.z = vectorSalida1.x;
            // trCar2.rotation.x = vectorSalida2.z;
            // trCar2.rotation.z = vectorSalida2.x;
            
            float aux = ccarCar1.speed;
            ccarCar1.speed = 5.f + ccarCar2.speed / 3.f;
            ccarCar2.speed = 5.f + aux / 3.f;*/
}

/**
 * Esta versión hace que los coches que han colisionado intercambien sus vectores de velocidad y rotación
 */
void CLPhysics::ExchangeVectors(CTransformable &trCar1, CCar &cCar1, CTransformable &trCar2, CCar &cCar2) {
    float aux = trCar1.rotation.y;
    trCar1.rotation.y = trCar2.rotation.y;
    trCar2.rotation.y = aux;

    aux = cCar1.speed;
    cCar1.speed = 5 + cCar2.speed / 2;
    cCar2.speed = 5 + aux / 2;
}

/**
 * Esta versión hace que los coches que han colisionado se reflejen entre sí
 */
void CLPhysics::ReflectCollision(CTransformable &trCar1, CCar &cCar1, CTransformable &trCar2, CCar &cCar2) {
    float anguloCar1 = trCar1.rotation.y;
    float anguloCar2 = trCar2.rotation.y;
    vec3 direccionCar;
    direccionCar.x = cos(anguloCar1 * M_PI / 180.0);
    direccionCar.y = 0;
    direccionCar.z = sin(anguloCar1 * M_PI / 180.0);

    vec3 direccionCarAI;
    direccionCarAI.x = cos(anguloCar2 * M_PI / 180.0);
    direccionCarAI.y = 0;
    direccionCarAI.z = sin(anguloCar2 * M_PI / 180.0);

    vec3 nuevaDirectionCar1 = glm::reflect(direccionCar, direccionCarAI);
    vec3 nuevaDirectionCar2 = glm::reflect(direccionCarAI, direccionCar);

    float nuevoAnguloCar1Rad = atan2(nuevaDirectionCar1.x, nuevaDirectionCar1.z);
    float nuevoAnguloCar1Deg = nuevoAnguloCar1Rad * (180.0 / M_PI);
    float nuevoAnguloCar2Rad = atan2(nuevaDirectionCar2.x, nuevaDirectionCar2.z);
    float nuevoAnguloCar2Deg = nuevoAnguloCar2Rad * (180.0 / M_PI);

    trCar1.rotation.y = nuevoAnguloCar1Deg;
    trCar2.rotation.y = nuevoAnguloCar2Deg;

    float aux = 5 + cCar1.speed / 3;
    cCar1.speed = 5 + cCar2.speed / 3;
    cCar2.speed = aux;
}

/**
 * Esta versión hace que los coches que han colisionado se reflejen entre sí
 */
void CLPhysics::ReflectSpherePlane(CTransformable &trCar1, CCar &cCar1, CBoundingPlane &plane) {
    if (cCar1.speed > 50.0) {
        cCar1.speed = -50.f - cCar1.speed / 6;
    } else {
        cCar1.speed = 25.0;
    }
}
