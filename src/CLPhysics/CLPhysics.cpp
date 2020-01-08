#define _USE_MATH_DEFINES

#include "CLPhysics.h"
#include <iostream>
#include <sstream>
#include "../Components/CAABoundingBox.h"
#include "../Components/CBoundingPlane.h"
#include "../Components/CBoundingSphere.h"
#include "../Components/CCar.h"
#include "../Components/CColliding.h"
#include "../Components/CRay.h"
#include "../Components/CSpeed.h"
#include "../Components/CTransformable.h"
#include "../Entities/BoundingWall.h"
#include "../Entities/Car.h"
#include "../Entities/CarAI.h"
#include "../EventManager/Event.h"
#include "../EventManager/EventManager.h"
#include "../Managers/ManBoundingWall.h"
#include "../Managers/ManCar.h"
#include "../Managers/Manager.h"
#include "../Systems/Utils.h"

using namespace std;

CLPhysics::CLPhysics() {
}

void CLPhysics::AddManager(Manager &m) {
    managers.push_back(&m);
}

void CLPhysics::Update(float delta) {
    Simulate(delta);
    HandleCollisions();
    HandleCollisionsWithPlanes();
}

void CLPhysics::HandleCollisionsWithPlanes() {
    ManCar *manCar = static_cast<ManCar *>(managers[0]);
    ManBoundingWall *manWalls = static_cast<ManBoundingWall *>(managers[1]);

    Car *c = manCar->GetCar().get();

    vector<shared_ptr<CarAI>> carAIs = manCar->GetEntitiesAI();
    size_t numCarAIs = carAIs.size();

    vector<shared_ptr<Entity>> walls = manWalls->GetEntities();
    size_t numWalls = walls.size();

    CBoundingSphere *spc = static_cast<CBoundingSphere *>(c->GetComponent(CompType::CompBoundingSphere).get());
    CTransformable *trc = static_cast<CTransformable *>(c->GetComponent(CompType::TransformableComp).get());
    CCar *ccarc = static_cast<CCar *>(c->GetComponent(CompType::CarComp).get());

    // mi coche con todos los walls
    for (size_t currentWall = 0; currentWall < numWalls; currentWall++) {
        BoundingWall *wall = static_cast<BoundingWall *>(walls[currentWall].get());
        CBoundingPlane *plane = static_cast<CBoundingPlane *>(wall->GetComponent(CompType::CompBoundingPlane).get());
        HandleCollisions(*trc, *spc, *ccarc, true, *plane);
    }

    // las ias con los walls
    for (size_t currentAI = 0; currentAI < numCarAIs; currentAI++) {
        for (size_t currentWall = 0; currentWall < numWalls; currentWall++) {
            CarAI *car = manCar->GetEntitiesAI()[currentAI].get();
            BoundingWall *wall = static_cast<BoundingWall *>(walls[currentWall].get());

            CBoundingSphere *spcar1 = static_cast<CBoundingSphere *>(car->GetComponent(CompType::CompBoundingSphere).get());
            CTransformable *trcar1 = static_cast<CTransformable *>(car->GetComponent(CompType::TransformableComp).get());
            CCar *ccarcar1 = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());

            CBoundingPlane *plane = static_cast<CBoundingPlane *>(wall->GetComponent(CompType::CompBoundingPlane).get());
            HandleCollisions(*trcar1, *spcar1, *ccarcar1, false, *plane);
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

    Car *c = manCar->GetCar().get();

    vector<shared_ptr<CarAI>> entities = manCar->GetEntitiesAI();
    size_t numEntities = entities.size();

    CBoundingSphere *spc = static_cast<CBoundingSphere *>(c->GetComponent(CompType::CompBoundingSphere).get());
    CTransformable *trc = static_cast<CTransformable *>(c->GetComponent(CompType::TransformableComp).get());
    CCar *ccarc = static_cast<CCar *>(c->GetComponent(CompType::CarComp).get());

    // mi coche con todos los coches de AI
    for (size_t i = 0; i < numEntities; i++) {
        CarAI *cai = manCar->GetEntitiesAI()[i].get();
        CBoundingSphere *spcai = static_cast<CBoundingSphere *>(cai->GetComponent(CompType::CompBoundingSphere).get());
        CTransformable *trcai = static_cast<CTransformable *>(cai->GetComponent(CompType::TransformableComp).get());
        CCar *ccarcai = static_cast<CCar *>(cai->GetComponent(CompType::CarComp).get());
        HandleCollisions(*trc, *spc, *ccarc, true, *trcai, *spcai, *ccarcai);
    }

    // las ias entre sí
    for (size_t i = 0; i < numEntities; i++) {
        for (size_t j = i + 1; j < numEntities; j++) {
            CarAI *car1 = manCar->GetEntitiesAI()[i].get();
            CarAI *car2 = manCar->GetEntitiesAI()[j].get();
            CBoundingSphere *spcar1 = static_cast<CBoundingSphere *>(car1->GetComponent(CompType::CompBoundingSphere).get());
            CTransformable *trcar1 = static_cast<CTransformable *>(car1->GetComponent(CompType::TransformableComp).get());
            CCar *ccarcar1 = static_cast<CCar *>(car1->GetComponent(CompType::CarComp).get());

            CBoundingSphere *spcar2 = static_cast<CBoundingSphere *>(car2->GetComponent(CompType::CompBoundingSphere).get());
            CTransformable *trcar2 = static_cast<CTransformable *>(car2->GetComponent(CompType::TransformableComp).get());
            CCar *ccarcar2 = static_cast<CCar *>(car2->GetComponent(CompType::CarComp).get());

            HandleCollisions(*trcar1, *spcar1, *ccarcar1, false, *trcar2, *spcar2, *ccarcar2);
        }
    }
}

void CLPhysics::PositionSphereIntoTransformable(CTransformable &tr, CBoundingSphere &sp) {
    sp.center = tr.position;
    float x = -cos(Utils::DegToRad(tr.rotation.y)) * sp.radius;
    float z = sin(Utils::DegToRad(tr.rotation.y)) * sp.radius;
    sp.center.x += x;
    sp.center.z += z;
}

void CLPhysics::SeparateSpheres(CTransformable &trCar1, CBoundingSphere &spCar1, CCar &ccarCar1,
                                CTransformable &trCar2, CBoundingSphere &spCar2, CCar &ccarCar2) {
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

void CLPhysics::HandleCollisions(CTransformable &trCar, CBoundingSphere &spCar, CCar &ccarCar, bool mainCar, CBoundingPlane &plane) {
    PositionSphereIntoTransformable(trCar, spCar);
    IntersectData intersData = plane.IntersectSphere(spCar);
    if (intersData.intersects) {
        // SonarChoque(mainCar);
        SeparateSphereFromPlane(intersData, trCar, spCar, ccarCar, plane);
    }
}

void CLPhysics::SeparateSphereFromPlane(IntersectData &intersData, CTransformable &trCar1, CBoundingSphere &spCar1, CCar &ccarCar1, CBoundingPlane &plane) const {
    vec3 direction = spCar1.center - plane.normal;  // te da la dirección al otro bounding en x, y, z, es decir, si tenemos 200, 10, 30, significa que estamos a 200 de distancia en x, a 10 en y y a 30 en z
    vec3 nuevaDirectionCar1 = -normalize(direction);
    float correctedDistance = intersData.GetDistance();
    trCar1.position.x += nuevaDirectionCar1.x * correctedDistance;
    trCar1.position.z += nuevaDirectionCar1.z * correctedDistance;
}

void CLPhysics::SonarChoque(bool mainCar) {
    DataMap map;
    map["mainCharacter"] = mainCar;
    Event e(EventType::CRASH_ENEMY, map);
    EventManager::GetInstance()->AddEventMulti(e);
}

/**
 * Recibe los componentes de los dos coches con los que se comprobará colisión
 * El bool mainCar define si los componentes del coche1 son los del coche principal o no
 */
void CLPhysics::HandleCollisions(CTransformable &trCar1, CBoundingSphere &spCar1, CCar &ccarCar1, bool mainCar, CTransformable &trCar2, CBoundingSphere &spCar2, CCar &ccarCar2) {
    // posicionamos la esfera en la misma posición que el coche pero teniendo en cuenta el offset

    PositionSphereIntoTransformable(trCar1, spCar1);
    PositionSphereIntoTransformable(trCar2, spCar2);
    IntersectData intersData = spCar1.IntersectSphere(spCar2);
    if (intersData.intersects) {
        SonarChoque(mainCar);

        SeparateSpheres(trCar1, spCar1, ccarCar1, trCar2, spCar2, ccarCar2);
        PositionSphereIntoTransformable(trCar1, spCar1);
        PositionSphereIntoTransformable(trCar2, spCar2);

        float anguloCar1 = trCar1.rotation.y;
        float anguloCar2 = trCar2.rotation.y;
        float anguloEntreEllos = Utils::AngleBetweenTwoAngles(anguloCar1, anguloCar2);
        //cout << "angulo entre ellos=" << anguloEntreEllos << endl;
        if (anguloEntreEllos > 0 && anguloEntreEllos <= 45) {
            // intercambiamos velocidades pero el ángulo no se toca
            //cout << "Intercambiamos velocidades1" << endl;
            float aux = ccarCar1.speed;
            ccarCar1.speed = ccarCar2.speed;
            ccarCar2.speed = aux;
        } else if (anguloEntreEllos > 45 && anguloEntreEllos <= 115) {
            //cout << "chocan lateralmente" << endl;

            // versión intercambio de vectores
            // ExchangeVectors(trCar1, ccarCar2, trCar2, ccarCar2);

            // cout << "Intercambiamos velocidades2" << endl;
            // float aux = ccarCar1.speed;

            // ccarCar1.speed = ccarCar2.speed / 3;
            // ccarCar2.speed = aux / 3;

            // versión reflejo
            // ReflectCollision(trCar1, ccarCar1, trCar2, ccarCar2);
        } else if (anguloEntreEllos > 115) {
            // intercambiamos velocidades pero el ángulo no se toca
            //cout << "Intercambiamos velocidades2" << endl;
            float aux = ccarCar1.speed;

            ccarCar1.speed = -50.f - ccarCar2.speed / 6;
            ccarCar2.speed = -50.f - aux / 6;
        }
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