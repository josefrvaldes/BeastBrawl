#define _USE_MATH_DEFINES

#include "CLPhysics.h"
#include <iostream>
#include "../Components/CAABoundingBox.h"
#include "../Components/CBoundingPlane.h"
#include "../Components/CBoundingSphere.h"
#include "../Components/CSpeed.h"
#include "../Components/CTransformable.h"
#include "../Entities/Car.h"
#include "../Entities/CarAI.h"
#include "../Managers/ManCar.h"
#include "../Managers/Manager.h"

using namespace std;

CLPhysics::CLPhysics() {
}

void CLPhysics::AddManager(Manager &m) {
    managers.push_back(&m);
}

void CLPhysics::Update(float delta) {
    Simulate(delta);
    HandleCollisions();
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
    CBoundingSphere *spc = static_cast<CBoundingSphere *>(c->GetComponent(CompType::CompBoundingSphere).get());
    CTransformable *trc = static_cast<CTransformable *>(c->GetComponent(CompType::TransformableComp).get());
    spc->center = trc->position;

    vector<shared_ptr<CarAI>> entities = manCar->GetEntitiesAI();
    size_t numEntities = entities.size();
    // mi coche con todos los coches de AI
    for (size_t i = 0; i < numEntities; i++) {
        CarAI *cai = manCar->GetEntitiesAI()[i].get();
        CBoundingSphere *spcai = static_cast<CBoundingSphere *>(cai->GetComponent(CompType::CompBoundingSphere).get());
        CTransformable *trcai = static_cast<CTransformable *>(cai->GetComponent(CompType::TransformableComp).get());
        HandleCollisions(*trc, *spc, *trcai, *spcai);
    }

    // las ias entre sí
    for (size_t i = 0; i < numEntities; i++) {
        for (size_t j = i + 1; j < numEntities; j++) {
            CarAI *car1 = manCar->GetEntitiesAI()[i].get();
            CBoundingSphere *spcar1 = static_cast<CBoundingSphere *>(car1->GetComponent(CompType::CompBoundingSphere).get());
            CTransformable *trcar1 = static_cast<CTransformable *>(car1->GetComponent(CompType::TransformableComp).get());

            CarAI *car2 = manCar->GetEntitiesAI()[j].get();
            CBoundingSphere *spcar2 = static_cast<CBoundingSphere *>(car2->GetComponent(CompType::CompBoundingSphere).get());
            CTransformable *trcar2 = static_cast<CTransformable *>(car2->GetComponent(CompType::TransformableComp).get());
            HandleCollisions(*trcar1, *spcar1, *trcar2, *spcar2);
        }
    }
}

void CLPhysics::HandleCollisions(CTransformable &trCar1, CBoundingSphere &spCar1, CTransformable &trCar2, CBoundingSphere &spCar2) {
    spCar2.center = trCar2.position;
    IntersectData intersData = spCar1.IntersectSphere(spCar2);
    if (intersData.intersects) {
        float anguloCar = trCar1.rotation.y;
        vec3 direccionCar;
        direccionCar.x = cos(anguloCar * M_PI / 180.0);
        direccionCar.y = 0;
        direccionCar.z = sin(anguloCar * M_PI / 180.0);
        float anguloCarAI = trCar2.rotation.y;
        vec3 direccionCarAI;
        direccionCarAI.x = cos(anguloCarAI * M_PI / 180.0);
        direccionCarAI.y = 0;
        direccionCarAI.z = sin(anguloCarAI * M_PI / 180.0);

        vec3 nuevaDirectionCar1 = glm::reflect(direccionCar, direccionCarAI);
        vec3 nuevaDirectionCar2 = glm::reflect(direccionCarAI, direccionCar);

        float nuevoAnguloCar1Rad = atan2(nuevaDirectionCar1.x, nuevaDirectionCar1.z);
        float nuevoAnguloCar1Deg = nuevoAnguloCar1Rad * (180.0 / M_PI);
        float nuevoAnguloCar2Rad = atan2(nuevaDirectionCar2.x, nuevaDirectionCar2.z);
        float nuevoAnguloCar2Deg = nuevoAnguloCar2Rad * (180.0 / M_PI);

        trCar1.rotation.y = nuevoAnguloCar1Deg;
        trCar2.rotation.y = nuevoAnguloCar2Deg;
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

    CBoundingPlane plane1(vec3(0.f, 1.f, 0.f), 0.f);

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
         << endl;
}