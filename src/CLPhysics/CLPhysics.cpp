
#include "CLPhysics.h"
#include <iostream>
#include <memory>
#include "../Components/CExternalForce.h"
#include "../Components/CBoundingChassis.h"
#include "../Components/CBoundingCilindre.h"
#include "../Components/CBoundingOBB.h"
#include "../Entities/BoundingOBB.h"
#include "../Managers/ManBoundingOBB.h"

#include <Components/CAABoundingBox.h>
#include <Components/CBoundingPlane.h>
#include <Components/CBoundingSphere.h>
#include <Components/CCar.h>
#include <Components/CNitro.h>
#include <Components/CTransformable.h>
#include <Components/CTotem.h>
#include <Entities/BoundingWall.h>
#include <Entities/Car.h>
#include <Entities/CarAI.h>
#include <Entities/CarHuman.h>
#include <EventManager/Event.h>
#include <EventManager/EventManager.h>
#include <Managers/ManBoundingWall.h>
#include <Managers/ManCar.h>
#include <Managers/Manager.h>
#include <Systems/Utils.h>

#include <cmath>

using namespace std;

CLPhysics::CLPhysics() {
}

void CLPhysics::AddManager(Manager &m) {
    managers.push_back(&m);
}

void CLPhysics::Update(float delta) {
    Simulate(delta);
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ManCar *manCar = static_cast<ManCar *>(managers[0]);
    const auto& entities = manCar->GetEntities();
    size_t numEntities = entities.size();
    for (size_t i = 0; i < numEntities; i++) {
        Entity *car = manCar->GetEntities()[i].get();
        CTransformable *trcar = static_cast<CTransformable *>(car->GetComponent(CompType::TransformableComp).get());
        CBoundingChassis *cBoundingChassis = static_cast<CBoundingChassis *>(car->GetComponent(CompType::CompBoundingChassis).get());
        PositionSphBehindIntoTransf(*trcar, *cBoundingChassis->sphereBehind);
        PositionSphFrontIntoTransf(*trcar, *cBoundingChassis->sphereFront);
        PositionCilindreIntoSpheres(*cBoundingChassis);
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    HandleCollisions();             // COLISIONES ENTRE COCHES --> HECHO
    HandleCollisionsWithPlanes();   // COLISIONES COCHES PLANOS --> NO HECHO
    HandleCollisionsWithOBB();      // COLISIONES COCHES OBB --> NO HECHO

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (size_t i = 0; i < numEntities; i++) {
        Entity *car = manCar->GetEntities()[i].get();
        CTransformable *trcar = static_cast<CTransformable *>(car->GetComponent(CompType::TransformableComp).get());
        CBoundingChassis *cBoundingChassis = static_cast<CBoundingChassis *>(car->GetComponent(CompType::CompBoundingChassis).get());
        PositionSphBehindIntoTransf(*trcar, *cBoundingChassis->sphereBehind);
        PositionSphFrontIntoTransf(*trcar, *cBoundingChassis->sphereFront);
        PositionCilindreIntoSpheres(*cBoundingChassis);
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


}

void CLPhysics::HandleCollisionsWithOBB(){
    
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
        //CBoundingChassis *chaCar = static_cast<CBoundingChassis *>(car->GetComponent(CompType::CompBoundingChassis).get());
        CTransformable *trcar = static_cast<CTransformable *>(car->GetComponent(CompType::TransformableComp).get());
        CCar *ccarcar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());
        //CExternalForce *cExternalForce = static_cast<CExternalForce *>(car->GetComponent(CompType::CompExternalForce).get());
        for (size_t currentOBB = 0; currentOBB < numOBBs; currentOBB++) {
            //BoundingWall *wall = static_cast<BoundingWall *>(obbs[currentOBB].get());
            BoundingOBB *obbActual = static_cast<BoundingOBB *>(obbs[currentOBB].get());
            //CBoundingPlane *plane = static_cast<CBoundingPlane *>(wall->GetComponent(CompType::CompBoundingPlane).get());
            CBoundingOBB *cOBBactual = static_cast<CBoundingOBB *>(obbActual->GetComponent(CompType::CompBoundingOBB).get());
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            PositionSphereIntoTransformable(*trcar, *spcar);
            HandleCollisions(*trcar, *spcar, *ccarcar, false, *cOBBactual);
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            //CollisionsSphereOBB(*trcar, *chaCar, *ccarcar, false, *cOBBactual);
        }
    }
}

void CLPhysics::HandleCollisionsWithPlanes() {
    ManCar *manCar = static_cast<ManCar *>(managers[0]);
    ManBoundingWall *manWalls = static_cast<ManBoundingWall *>(managers[1]);

    const auto& carAIs = manCar->GetEntities();
    size_t numCar = carAIs.size();

    auto walls = manWalls->GetEntities();
    size_t numWalls = walls.size();

    // los coches con los walls
    for (size_t currentAI = 0; currentAI < numCar; currentAI++) {
        Entity *car = manCar->GetEntities()[currentAI].get();
        //CBoundingSphere *spcar = static_cast<CBoundingSphere *>(car->GetComponent(CompType::CompBoundingSphere).get());
        CBoundingChassis *chaCar = static_cast<CBoundingChassis *>(car->GetComponent(CompType::CompBoundingChassis).get());
        CTransformable *trcar = static_cast<CTransformable *>(car->GetComponent(CompType::TransformableComp).get());
        CCar *ccarcar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());
        for (size_t currentWall = 0; currentWall < numWalls; currentWall++) {
            BoundingWall *wall = static_cast<BoundingWall *>(walls[currentWall].get());
            CBoundingPlane *plane = static_cast<CBoundingPlane *>(wall->GetComponent(CompType::CompBoundingPlane).get());

            //HandleCollisions(*trcar, *spcar, *ccarcar, false, *plane);
            CollisionsSpherePlane(*trcar, *chaCar, *ccarcar, false, *plane);
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
    ManCar *manCar = static_cast<ManCar *>(managers[0]);

    const auto& entities = manCar->GetEntities();
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
            intersect = CollisionsCilindreSphere(*trcar1, *ccarcar1, *chacar1, *trcar2, *ccarcar1, *chacar2);
            if(!intersect){
                // ESFERAS
                intersect  = CollisionsSphereSphere(*trcar1, *chacar1, *ccarcar1, false, *trcar2, *chacar2, *ccarcar2, *cExternalForce1, *cExternalForce2);
            }
            if(intersect){
                checkCollisionNitro(car1, car2);
            }
        }
    }
}

void CLPhysics::checkCollisionNitro(Entity* car1, Entity* car2){
    //vamos a comprobar si una de las dos llevaba el nitro y en cuyo caso si la otra el totem para quitarselo
    auto cNitroCar1 = static_cast<CNitro *>(car1->GetComponent(CompType::NitroComp).get());
    if(cNitroCar1->activePowerUp){
        // si el primero tiene super nitro vemos si elsegundo tiene el totem
        auto cTotemCar2 = static_cast<CTotem *>(car2->GetComponent(CompType::TotemComp).get());
        if(cTotemCar2->active){
            // si que lo tiene... lanzamos evento para intercambiarlo
            shared_ptr<DataMap> data = make_shared<DataMap>();   
            (*data)[CAR_WITH_TOTEM] = car2;     
            (*data)[CAR_WITHOUT_TOTEM] = car1;                                                                                                    
            EventManager::GetInstance().AddEventMulti(Event{EventType::CHANGE_TOTEM_CAR, data}); 
        }
    
    }else{
        auto cNitroCar2 = static_cast<CNitro *>(car2->GetComponent(CompType::NitroComp).get());
        if(cNitroCar2->activePowerUp){
            auto cTotemCar1 = static_cast<CTotem *>(car1->GetComponent(CompType::TotemComp).get());
            if(cTotemCar1->active){
                // si que lo tiene... lanzamos evento para intercambiarlo
                shared_ptr<DataMap> data = make_shared<DataMap>();   
                      
                (*data)[CAR_WITH_TOTEM] = car1;     
                (*data)[CAR_WITHOUT_TOTEM] = car2;                                                                                                    
                EventManager::GetInstance().AddEventMulti(Event{EventType::CHANGE_TOTEM_CAR, data}); 
            }
        }
    }
}

void CLPhysics::PositionSphereIntoTransformable(CTransformable &tr, CBoundingSphere &sp) {
    sp.center = tr.position;
    float x = -cos(Utils::DegToRad(tr.rotation.y)) * (sp.radius);
    float z = sin(Utils::DegToRad(tr.rotation.y)) * (sp.radius);
    sp.center.x += x;
    sp.center.z += z;
}


void CLPhysics::PositionSphBehindIntoTransf(CTransformable &tr, CBoundingSphere &sp) const{
    sp.center = tr.position;
    float x = -cos(Utils::DegToRad(tr.rotation.y)) * (sp.radius);
    float z = sin(Utils::DegToRad(tr.rotation.y)) * (sp.radius);
    sp.center.x += x;
    sp.center.z += z;
    // Necesitamos tambien actualizar el CILINDRO
}

void CLPhysics::PositionSphFrontIntoTransf(CTransformable &tr, CBoundingSphere &sp) const{
    sp.center = tr.position;
    float x = -cos(Utils::DegToRad(tr.rotation.y)) * (sp.radius+10);
    float z = sin(Utils::DegToRad(tr.rotation.y)) * (sp.radius+10);
    sp.center.x += x;
    sp.center.z += z;
    // Necesitamos tambien actualizar el cilindro
}

void CLPhysics::PositionCilindreIntoSpheres(CBoundingChassis &chassis) const{
    chassis.cilindre->extreme1 = chassis.sphereBehind->center;
    chassis.cilindre->extreme2 = chassis.sphereFront->center;
}


bool CLPhysics::CollisionsCilindreSphere(CTransformable &trCar1, CCar &ccar1, CBoundingChassis &cChaCar1, CTransformable &trCar2, CCar &ccar2, CBoundingChassis &cChaCar2){
    //cout << "entramos"<< endl;
    IntersectData inters1 = cChaCar1.cilindre->IntersectSphere(*(cChaCar2.sphereBehind.get()));
    if(inters1.intersects){
        // CORREGIMOS LA POSICION
        SeparateCilindreSphere(trCar1, inters1.virtualCenter, inters1.virtualRadius, ccar1, trCar2, cChaCar2.sphereBehind->center, cChaCar2.sphereBehind->radius, ccar2);
        return true;
    }
    IntersectData inters2 = cChaCar1.cilindre->IntersectSphere(*(cChaCar2.sphereFront.get()));
    if(inters2.intersects){
        // CORREGIMOS LA POSICION
        SeparateCilindreSphere(trCar1, inters2.virtualCenter, inters2.virtualRadius, ccar1, trCar2, cChaCar2.sphereFront->center, cChaCar2.sphereFront->radius, ccar2);
        return true;
    }
    IntersectData inters3 = cChaCar2.cilindre->IntersectSphere(*(cChaCar1.sphereBehind.get()));
    if(inters3.intersects){
        // CORREGIMOS LA POSICION
        SeparateCilindreSphere(trCar2, inters3.virtualCenter, inters3.virtualRadius, ccar2, trCar1, cChaCar1.sphereBehind->center, cChaCar1.sphereBehind->radius, ccar1);
        return true;
    }
    IntersectData inters4 = cChaCar2.cilindre->IntersectSphere(*(cChaCar1.sphereFront.get()));
    if(inters4.intersects){
        // CORREGIMOS LA POSICION
        SeparateCilindreSphere(trCar2, inters4.virtualCenter, inters4.virtualRadius, ccar2, trCar1, cChaCar1.sphereFront->center, cChaCar1.sphereFront->radius, ccar1);
        return true;
    }
    return false;

}


void CLPhysics::CollisionsSpherePlane(CTransformable &trCar, CBoundingChassis &chaCar, CCar &ccar, bool mainCar, CBoundingPlane &plane){
    
    CBoundingSphere *spBehindCar = chaCar.sphereBehind.get();
    bool intersect = false;
    //PositionSphBehindIntoTransf(trCar, *spBehindCar);
    intersect = HandleCollisions(trCar, *spBehindCar, ccar, false, plane);
    if(!intersect){
        CBoundingSphere *spFrontCar = chaCar.sphereFront.get();
        //PositionSphFrontIntoTransf(trCar, *spFrontCar);
        HandleCollisions(trCar, *spFrontCar, ccar, false, plane);
    }
}



// TODO: Relamente no deberia de ser /2 debera de depender de la fuerza con la que colisionan.... sino puede llegar a temblar, o desplazar
void CLPhysics::SeparateSpheres(CTransformable &trCar1, CBoundingSphere &spCar1, CCar &ccarCar1,
                                CTransformable &trCar2, CBoundingSphere &spCar2, CCar &ccarCar2) const{
    vec3 direction = spCar2.center - spCar1.center;  // te da la dirección al otro bounding en x, y, z, es decir, si tenemos 200, 10, 30, significa que estamos a 200 de distancia en x, a 10 en y y a 30 en z
    float centerDistance = glm::length(direction);
    float distanceCollided = abs(spCar1.radius + spCar2.radius - centerDistance);

    // vec3 nuevaDirectionCar1 = glm::reflect(-direction, direction);
    // vec3 nuevaDirectionCar2 = glm::reflect(direction, -direction);

    vec3 nuevaDirectionCar1 = -normalize(direction);
    vec3 nuevaDirectionCar2 = normalize(direction);

    trCar1.position.x += nuevaDirectionCar1.x * (distanceCollided / 2);
    trCar1.position.z += nuevaDirectionCar1.z * (distanceCollided / 2);
    trCar2.position.x += nuevaDirectionCar2.x * (distanceCollided / 2);
    trCar2.position.z += nuevaDirectionCar2.z * (distanceCollided / 2);
}

void CLPhysics::SeparateCilindreSphere(CTransformable &trCar1, glm::vec3 &cenCar1, const float &radCar1, CCar &ccarCar1, CTransformable &trCar2, glm::vec3 &cenCar2, const float &radCar2, CCar &ccarCar2) const{
    vec3 direction = cenCar2 - cenCar1;  // te da la dirección al otro bounding en x, y, z, es decir, si tenemos 200, 10, 30, significa que estamos a 200 de distancia en x, a 10 en y y a 30 en z
    float centerDistance = glm::length(direction);
    float distanceCollided = abs(radCar1 + radCar2 - centerDistance);

    // vec3 nuevaDirectionCar1 = glm::reflect(-direction, direction);
    // vec3 nuevaDirectionCar2 = glm::reflect(direction, -direction);

    vec3 nuevaDirectionCar1 = -normalize(direction);
    vec3 nuevaDirectionCar2 = normalize(direction);

    trCar1.position.x += nuevaDirectionCar1.x * (distanceCollided / 2);
    trCar1.position.z += nuevaDirectionCar1.z * (distanceCollided / 2);
    trCar2.position.x += nuevaDirectionCar2.x * (distanceCollided / 2);
    trCar2.position.z += nuevaDirectionCar2.z * (distanceCollided / 2);
}


// HandleCollisions entre ESFERA de un COCHE y un PLANO 
bool CLPhysics::HandleCollisions(CTransformable &trCar, CBoundingSphere &spCar, CCar &ccarCar, bool mainCar, CBoundingPlane &plane) {
    //PositionSphereIntoTransformable(trCar, spCar);
    IntersectData intersData = plane.IntersectSphere(spCar, trCar, ccarCar);
    if (intersData.intersects) {
        // SonarChoque(mainCar);
        SeparateSphereFromPlane(intersData, trCar, spCar, ccarCar, plane);


        vec3 vecDirCar = CalculateVecDirCar(trCar);
        double angle2V = Angle2Vectors( vecDirCar, plane.normalizedNormal);
        //cout << angle2V << endl;
        if(angle2V > 130.0){ // no debemos reflejar del todo
            if(ccarCar.speed >= ccarCar.maxSpeed*0.3){
            ccarCar.speed = ccarCar.maxSpeed*0.3;
            }
        }else{
            if(ccarCar.speed >= ccarCar.maxSpeed*0.8){
            ccarCar.speed = ccarCar.maxSpeed*0.8;
            }     
        }
        //ReflectSpherePlane(trCar, ccarCar, plane);

        //TODO: PROVISIONAL ... PONER LA VELOCIDAD DEL COCHE A 0
        //ccarCar.speed = ccarCar.speed/1.01;
        return true;
    }
    return false;
}


// HandleCollisions entre ESFERA de un COCHE y un OBB 
bool CLPhysics::HandleCollisions(CTransformable &trCar, CBoundingSphere &spCar, CCar &ccarCar, bool mainCar, CBoundingOBB &obb) {
    //PositionSphereIntoTransformable(trCar, spCar);
    IntersectData intersData = obb.IntersectSphere(spCar, trCar, ccarCar);
    if (intersData.intersects) {
        // SonarChoque(mainCar);
        SeparateSphereFromPlane(intersData, trCar, spCar, ccarCar, *obb.planes[intersData.posEntity] );

        // dependiendo de como colisionemos con el plano sera un tipo de colision u otro.
        // dependera del angulo de colsion y la velocidad de colision
        
        vec3 vecDirCar = CalculateVecDirCar(trCar);
        double angle2V = Angle2Vectors( vecDirCar, obb.planes[intersData.posEntity]->normalizedNormal);
        //cout << angle2V << endl;
        if(angle2V > 130.0){ // no debemos reflejar del todo
            if(ccarCar.speed >= ccarCar.maxSpeed*0.3){
            ccarCar.speed = ccarCar.maxSpeed*0.3;
            }
        }else{
            if(ccarCar.speed >= ccarCar.maxSpeed*0.8){
            ccarCar.speed = ccarCar.maxSpeed*0.8;
            }     
        }
        return true;
    }
    return false;
}


// TODO: no hay colisiones entre CILINDRO-PLANO
void CLPhysics::CollisionsSphereOBB(CTransformable &trCar, CBoundingChassis &chaCar, CCar &ccar, bool mainCar, CBoundingOBB &obb){
    
    CBoundingSphere *spBehindCar = chaCar.sphereBehind.get();
    bool intersect = false;
    //PositionSphBehindIntoTransf(trCar, *spBehindCar);
    intersect = HandleCollisions(trCar, *spBehindCar, ccar, false, obb);
    if(!intersect){
        CBoundingSphere *spFrontCar = chaCar.sphereFront.get();
        //PositionSphFrontIntoTransf(trCar, *spFrontCar);
        HandleCollisions(trCar, *spFrontCar, ccar, false, obb);
    }
}

vec3 CLPhysics::CalculateVecDirCar(CTransformable &cTransformable) const{

   float angleRotation = (cTransformable.rotation.y * M_PI) / 180.0;
   float nextPosX    = cTransformable.position.x - cos(angleRotation) * 1;
   float nexPosZ     = cTransformable.position.z + sin(angleRotation) * 1;

   return vec3(nextPosX-cTransformable.position.x, 0, nexPosZ-cTransformable.position.z);

}


void CLPhysics::SeparateSphereFromPlane(IntersectData &intersData, CTransformable &trCar1, CBoundingSphere &spCar1, CCar &ccarCar1, CBoundingPlane &plane) const {
    //cout << "NO SEPARO" << endl;
    vec3 direction = spCar1.center - plane.normal;  // te da la dirección al otro bounding en x, y, z, es decir, si tenemos 200, 10, 30, significa que estamos a 200 de distancia en x, a 10 en y y a 30 en z
    vec3 nuevaDirectionCar1 = -normalize(direction);
    float correctedDistance = intersData.GetDistance();
    trCar1.position.x += nuevaDirectionCar1.x * correctedDistance;
    trCar1.position.z += nuevaDirectionCar1.z * correctedDistance;
}

void CLPhysics::SonarChoque(bool mainCar) {
    shared_ptr<DataMap> map = make_shared<DataMap>();
    (*map)[MAIN_CAR] = mainCar;
    Event e(EventType::CRASH_ENEMY, map);
    EventManager::GetInstance().AddEventMulti(e);
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
        //SonarChoque(mainCar);

        SeparateSpheres(trCar1, spCar1, ccarCar1, trCar2, spCar2, ccarCar2);
        PositionSphereIntoTransformable(trCar1, spCar1);
        PositionSphereIntoTransformable(trCar2, spCar2);

        // LO QUE HACEMOS ES PASAR NUESTRO VECTOR DE FUERZA AL COCHE CONTRARIO
        ReciveExternalForce(trCar1, ccarCar1, cExtForc1, trCar2, ccarCar2, cExtForc2);

        // ADEMAS DEBEMOS REDUCIRLES LAS VELOCIDADES POR CHOCAR
        impactCarCar(ccarCar1, ccarCar2);

        //float anguloCar1 = trCar1.rotation.y;
        //float anguloCar2 = trCar2.rotation.y;
        //float anguloEntreEllos = Utils::AngleBetweenTwoAngles(anguloCar1, anguloCar2);
        //cout << "angulo entre ellos=" << anguloEntreEllos << endl;
        //if (anguloEntreEllos > 0 && anguloEntreEllos <= 45) {
        //    // intercambiamos velocidades pero el ángulo no se toca
        //    //cout << "Intercambiamos velocidades1" << endl;
        //    float aux = ccarCar1.speed;
        //    ccarCar1.speed = ccarCar2.speed;
        //    ccarCar2.speed = aux;
        // } else if (anguloEntreEllos > 45 && anguloEntreEllos <= 115) {
            //cout << "chocan lateralmente" << endl;

            // versión intercambio de vectores
            // ExchangeVectors(trCar1, ccarCar2, trCar2, ccarCar2);

            // cout << "Intercambiamos velocidades2" << endl;
            // float aux = ccarCar1.speed;

            // ccarCar1.speed = ccarCar2.speed / 3;
            // ccarCar2.speed = aux / 3;

            // versión reflejo
            // ReflectCollision(trCar1, ccarCar1, trCar2, ccarCar2);
        //} else if (anguloEntreEllos > 115) {
        //    // intercambiamos velocidades pero el ángulo no se toca
        //    //cout << "Intercambiamos velocidades2" << endl;
        //    float aux = ccarCar1.speed;
//
        //    ccarCar1.speed = -50.f - ccarCar2.speed / 6;
        //    ccarCar2.speed = -50.f - aux / 6;
        //}

        

        return true;    // los coches han colisionado -- lo utilizamos para robar el totem si llevamos el nitro
    }
    return false;  // los coches no han colisionado
}

bool CLPhysics::CollisionsSphereSphere(CTransformable &trCar1, CBoundingChassis &chaCar1, CCar &ccar1, bool mainCar, CTransformable &trCar2, CBoundingChassis &chaCar2, CCar &ccar2, CExternalForce &cExtForc1, CExternalForce &cExtForc2){

    CBoundingSphere *spBehindCar1 = chaCar1.sphereBehind.get();
    CBoundingSphere *spFrontCar1 = chaCar1.sphereFront.get();

    CBoundingSphere *spBehindCar2 = chaCar2.sphereBehind.get();
    CBoundingSphere *spFrontCar2 = chaCar2.sphereFront.get();

    bool intersect = false;
    intersect = CollisionsFrontFront(trCar1, *spFrontCar1, ccar1, mainCar, trCar2, *spFrontCar2, ccar2, cExtForc1, cExtForc2);
    if(!intersect){
        intersect = CollisionsFrontBehind(trCar1, *spFrontCar1, ccar1, mainCar, trCar2, *spBehindCar2, ccar2, cExtForc1, cExtForc2);
        if(!intersect){
            intersect = CollisionsBehindFront(trCar1, *spBehindCar1, ccar1, mainCar, trCar2, *spFrontCar2, ccar2, cExtForc1, cExtForc2);
            if(!intersect){
                intersect = CollisionsBehindBehind(trCar1, *spBehindCar1, ccar1, mainCar, trCar2, *spBehindCar2, ccar2, cExtForc1, cExtForc2);
            }
        }
    }
    return intersect;
}

bool CLPhysics::CollisionsBehindBehind(CTransformable &trCar1, CBoundingSphere &spCar1, CCar &ccarCar1, bool mainCar, CTransformable &trCar2, CBoundingSphere &spCar2, CCar &ccarCar2, CExternalForce &cExtForc1, CExternalForce &cExtForc2){
    //PositionSphBehindIntoTransf(trCar1, spCar1);
    //PositionSphBehindIntoTransf(trCar2, spCar2);
    IntersectData intersData = spCar1.IntersectSphere(spCar2);
    if (intersData.intersects) {
        //SonarChoque(mainCar);

        SeparateSpheres(trCar1, spCar1, ccarCar1, trCar2, spCar2, ccarCar2);
        //PositionSphBehindIntoTransf(trCar1, spCar1);
        //PositionSphBehindIntoTransf(trCar2, spCar2);

        ReciveExternalForce(trCar1, ccarCar1, cExtForc1, trCar2, ccarCar2, cExtForc2);
        impactCarCar(ccarCar1, ccarCar2);

        return true;
    }
    return false; 
}

bool CLPhysics::CollisionsBehindFront(CTransformable &trCar1, CBoundingSphere &spCar1, CCar &ccarCar1, bool mainCar, CTransformable &trCar2, CBoundingSphere &spCar2, CCar &ccarCar2, CExternalForce &cExtForc1, CExternalForce &cExtForc2){
    // posicionamos la esfera en la misma posición que el coche pero teniendo en cuenta el offset

    //PositionSphBehindIntoTransf(trCar1, spCar1);
    //PositionSphFrontIntoTransf(trCar2, spCar2);
    IntersectData intersData = spCar1.IntersectSphere(spCar2);
    if (intersData.intersects) {
        //SonarChoque(mainCar);

        SeparateSpheres(trCar1, spCar1, ccarCar1, trCar2, spCar2, ccarCar2);
        //PositionSphBehindIntoTransf(trCar1, spCar1);
        //PositionSphFrontIntoTransf(trCar2, spCar2);

        ReciveExternalForce(trCar1, ccarCar1, cExtForc1, trCar2, ccarCar2, cExtForc2);
        impactCarCar(ccarCar1, ccarCar2);

        return true;    // los coches han colisionado -- lo utilizamos para robar el totem si llevamos el nitro
    }
    return false;  // los coches no han colisionado  
}

bool CLPhysics::CollisionsFrontBehind(CTransformable &trCar1, CBoundingSphere &spCar1, CCar &ccarCar1, bool mainCar, CTransformable &trCar2, CBoundingSphere &spCar2, CCar &ccarCar2, CExternalForce &cExtForc1, CExternalForce &cExtForc2){
    // posicionamos la esfera en la misma posición que el coche pero teniendo en cuenta el offset

    //PositionSphFrontIntoTransf(trCar1, spCar1);
    //PositionSphBehindIntoTransf(trCar2, spCar2);
    IntersectData intersData = spCar1.IntersectSphere(spCar2);
    if (intersData.intersects) {
        //SonarChoque(mainCar);

        SeparateSpheres(trCar1, spCar1, ccarCar1, trCar2, spCar2, ccarCar2);
        //PositionSphFrontIntoTransf(trCar1, spCar1);
        //PositionSphBehindIntoTransf(trCar2, spCar2);

        ReciveExternalForce(trCar1, ccarCar1, cExtForc1, trCar2, ccarCar2, cExtForc2);
        impactCarCar(ccarCar1, ccarCar2);

        return true;    // los coches han colisionado -- lo utilizamos para robar el totem si llevamos el nitro
    }
    return false;  // los coches no han colisionado  
}

bool CLPhysics::CollisionsFrontFront(CTransformable &trCar1, CBoundingSphere &spCar1, CCar &ccarCar1, bool mainCar, CTransformable &trCar2, CBoundingSphere &spCar2, CCar &ccarCar2, CExternalForce &cExtForc1, CExternalForce &cExtForc2){
    // posicionamos la esfera en la misma posición que el coche pero teniendo en cuenta el offset

    //PositionSphFrontIntoTransf(trCar1, spCar1);
    //PositionSphFrontIntoTransf(trCar2, spCar2);
    IntersectData intersData = spCar1.IntersectSphere(spCar2);
    if (intersData.intersects) {
        //SonarChoque(mainCar);

        SeparateSpheres(trCar1, spCar1, ccarCar1, trCar2, spCar2, ccarCar2);
        //PositionSphFrontIntoTransf(trCar1, spCar1);
        //PositionSphFrontIntoTransf(trCar2, spCar2);

        ReciveExternalForce(trCar1, ccarCar1, cExtForc1, trCar2, ccarCar2, cExtForc2);
        impactCarCar(ccarCar1, ccarCar2);

        return true;    // los coches han colisionado -- lo utilizamos para robar el totem si llevamos el nitro
    }
    return false;  // los coches no han colisionado  
}

void CLPhysics::impactCarCar(CCar &ccar1, CCar &ccar2) const{
    if(ccar1.speed >= ccar1.maxSpeed*0.5){
        ccar1.speed = ccar1.maxSpeed*0.5;
    }
    if(ccar2.speed >= ccar2.maxSpeed*0.5){
        ccar2.speed = ccar2.maxSpeed*0.5;
    }
}

void CLPhysics::ReciveExternalForce(CTransformable &trCar1,  CCar &ccar1, CExternalForce &cExtForc1, CTransformable &trCar2,  CCar &ccar2, CExternalForce &cExtForc2){
    vec3 vecDirCar1 = CalculateVecDirCar(trCar1);
    vec3 vecDirCar2 = CalculateVecDirCar(trCar2);
    if( ccar2.speed > ccar2.maxSpeed*0.1){
        cExtForc1.dirExternalForce = vecDirCar2;
        cExtForc1.force = ccar2.speed; 
    }
    if(ccar1.speed > ccar1.maxSpeed*0.1){
        cExtForc2.dirExternalForce = vecDirCar1;
        cExtForc2.force = ccar1.speed;
    }
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
    //cout << " 1 1 1 1" << endl ;
    /*
    float anguloCar1 = trCar1.rotation.y;
    vec3 direccionCar;
    direccionCar.x = cos(anguloCar1 * M_PI / 180.0);
    direccionCar.y = 0;
    direccionCar.z = sin(anguloCar1 * M_PI / 180.0);

    // pondremos la normal del plano
    vec3 nuevaDirectionCar1 = glm::reflect(direccionCar, plane.normal);

    float nuevoAnguloCar1Rad = atan2(nuevaDirectionCar1.x, nuevaDirectionCar1.z);
    float nuevoAnguloCar1Deg = nuevoAnguloCar1Rad * (180.0 / M_PI);

    trCar1.rotation.y = nuevoAnguloCar1Deg;

    cCar1.speed = 5 + cCar1.speed / 3;
*/
    
   if(cCar1.speed > 50.0){
        cCar1.speed = -50.f - cCar1.speed / 6;
   }else{
       cCar1.speed = 25.0;
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
    cout << "sp1 intersects sp2: " << sp1sp2.intersects << ", Distance: " << sp1sp2.GetDistance() << endl;
    cout << "sp1 intersects sp3: " << sp1sp3.intersects << ", Distance: " << sp1sp3.GetDistance() << endl;
    cout << "sp1 intersects sp4: " << sp1sp4.intersects << ", Distance: " << sp1sp4.GetDistance() << endl
         << endl;

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

    cout << "AABB1 intersects AABB2: " << aabb1IDinters2.intersects << ", Distance: " << aabb1IDinters2.GetDistance() << endl;
    cout << "AABB1 intersects AABB3: " << aabb1IDinters3.intersects << ", Distance: " << aabb1IDinters3.GetDistance() << endl;
    cout << "AABB1 intersects AABB4: " << aabb1IDinters4.intersects << ", Distance: " << aabb1IDinters4.GetDistance() << endl;
    cout << "AABB1 intersects AABB5: " << aabb1IDinters5.intersects << ", Distance: " << aabb1IDinters5.GetDistance() << endl
         << endl;

    /*CBoundingPlane plane1(vec3(0.f, 1.f, 0.f), 0.f);

    IntersectData pl1sp1 = plane1.IntersectSphere(sp1);
    IntersectData pl1sp2 = plane1.IntersectSphere(sp2);
    IntersectData pl1sp3 = plane1.IntersectSphere(sp3);
    IntersectData pl1sp4 = plane1.IntersectSphere(sp4);

    assert(pl1sp1.intersects == true);
    assert(pl1sp1.GetDistance() == -1);

    assert(pl1sp2.intersects == false);
    assert(pl1sp2.GetDistance() == 2);

    assert(pl1sp3.intersects == true);
    assert(pl1sp3.GetDistance() == -1);

    assert(pl1sp4.intersects == true);
    assert(pl1sp4.GetDistance() == -1);

    cout << "plane1 intersects sphere1: " << pl1sp1.intersects << ", Distance: " << pl1sp1.GetDistance() << endl;
    cout << "plane1 intersects sphere2: " << pl1sp2.intersects << ", Distance: " << pl1sp2.GetDistance() << endl;
    cout << "plane1 intersects sphere3: " << pl1sp3.intersects << ", Distance: " << pl1sp3.GetDistance() << endl;
    cout << "plane1 intersects sphere4: " << pl1sp4.intersects << ", Distance: " << pl1sp4.GetDistance() << endl
         << endl;*/
}






IntersectData CLPhysics::HandleCollisionsRayWithSpheres(CTransformable &trCar1, CTransformable &trCar2, CBoundingSphere &spCar2, const glm::vec3 &normalRay){
    PositionSphereIntoTransformable(trCar2, spCar2);
    IntersectData intersData = spCar2.IntersectRay(trCar1, normalRay);

    return intersData;   
}

IntersectData CLPhysics::HandleCollisionsRayWithPlane(CTransformable &trRayOrigin,  glm::vec3 &rayNormalNormalized, CBoundingPlane &planeObject){
    glm::vec3 positionRayOrigin(trRayOrigin.position.x, trRayOrigin.position.y, trRayOrigin.position.z);
    IntersectData intersData = planeObject.IntersectRay(positionRayOrigin, rayNormalNormalized);

    return intersData;
}


double CLPhysics::Angle2Vectors(const vec3 &a, const vec3 &b) const{
    vec3 aN = glm::normalize(a);
    vec3 bN = glm::normalize(b);

    double dot = glm::dot(aN,bN);
    // Force the dot product of the two input vectors to
    // fall within the domain for inverse cosine, which
    // is -1 <= x <= 1. This will prevent runtime
    // "domain error" math exceptions.
    dot = ( dot < -1.0 ? -1.0 : ( dot > 1.0 ? 1.0 : dot ) );

    double angleRad = acos( dot );
    // grados = radianes*(180/PI_)
    return angleRad*(180/M_PI);  
}
