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
    CarAI *cai = manCar->GetEntitiesAI()[0].get();
    CBoundingSphere *spc = static_cast<CBoundingSphere *>(c->GetComponent(CompType::CompBoundingSphere).get());
    const CTransformable *trc = static_cast<CTransformable *>(c->GetComponent(CompType::TransformableComp).get());
    CBoundingSphere *spcai = static_cast<CBoundingSphere *>(cai->GetComponent(CompType::CompBoundingSphere).get());
    const CTransformable *trcai = static_cast<CTransformable *>(cai->GetComponent(CompType::TransformableComp).get());
    spc->center = trc->position;
    spcai->center = trcai->position;
    IntersectData intersData = spc->IntersectSphere(*spcai);
    if (intersData.intersects) {
        cout << "Intersecta hijodeputa" << endl;
    }
}

// void CLPhysics::Integrate(Entity &e, float delta) {
//     CTransformable *pos = static_cast<CTransformable *>(e.GetComponent(CompType::TransformableComp).get());
//     CSpeed *speed = static_cast<CSpeed *>(e.GetComponent(CompType::SpeedComp).get());
//     pos->position = pos->position + speed->speed * delta;
// }

void CLPhysics::RunTests() {
    CBoundingSphere sp1(vec3(0.f, 0.f, 0.f), 1.f);
    CBoundingSphere sp2(vec3(0.f, 3.f, 0.f), 1.f);
    CBoundingSphere sp3(vec3(0.f, 0.f, 2.f), 1.f);
    CBoundingSphere sp4(vec3(1.f, 0.f, 0.f), 1.f);

    IntersectData sp1sp2 = sp1.IntersectSphere(sp2);
    IntersectData sp1sp3 = sp1.IntersectSphere(sp3);
    IntersectData sp1sp4 = sp1.IntersectSphere(sp4);

    assert(sp1sp2.intersects == false);
    assert(sp1sp2.distance == 1);

    assert(sp1sp3.intersects == false);
    assert(sp1sp3.distance == 0);

    assert(sp1sp4.intersects == true);
    assert(sp1sp4.distance == -1);
    cout << "sp1 intersects sp2: " << sp1sp2.intersects << ", Distance: " << sp1sp2.distance << endl;
    cout << "sp1 intersects sp3: " << sp1sp3.intersects << ", Distance: " << sp1sp3.distance << endl;
    cout << "sp1 intersects sp4: " << sp1sp4.intersects << ", Distance: " << sp1sp4.distance << endl
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
    assert(aabb1IDinters2.distance == 0);

    assert(aabb1IDinters3.intersects == false);
    assert(aabb1IDinters3.distance == 0);

    assert(aabb1IDinters4.intersects == false);
    assert(aabb1IDinters4.distance == 1);

    assert(aabb1IDinters5.intersects == true);
    assert(aabb1IDinters5.distance == -0.5);

    cout << "AABB1 intersects AABB2: " << aabb1IDinters2.intersects << ", Distance: " << aabb1IDinters2.distance << endl;
    cout << "AABB1 intersects AABB3: " << aabb1IDinters3.intersects << ", Distance: " << aabb1IDinters3.distance << endl;
    cout << "AABB1 intersects AABB4: " << aabb1IDinters4.intersects << ", Distance: " << aabb1IDinters4.distance << endl;
    cout << "AABB1 intersects AABB5: " << aabb1IDinters5.intersects << ", Distance: " << aabb1IDinters5.distance << endl
         << endl;

    CBoundingPlane plane1(vec3(0.f, 1.f, 0.f), 0.f);

    IntersectData pl1sp1 = plane1.IntersectSphere(sp1);
    IntersectData pl1sp2 = plane1.IntersectSphere(sp2);
    IntersectData pl1sp3 = plane1.IntersectSphere(sp3);
    IntersectData pl1sp4 = plane1.IntersectSphere(sp4);

    assert(pl1sp1.intersects == true);
    assert(pl1sp1.distance == -1);

    assert(pl1sp2.intersects == false);
    assert(pl1sp2.distance == 2);

    assert(pl1sp3.intersects == true);
    assert(pl1sp3.distance == -1);

    assert(pl1sp4.intersects == true);
    assert(pl1sp4.distance == -1);

    cout << "plane1 intersects sphere1: " << pl1sp1.intersects << ", Distance: " << pl1sp1.distance << endl;
    cout << "plane1 intersects sphere2: " << pl1sp2.intersects << ", Distance: " << pl1sp2.distance << endl;
    cout << "plane1 intersects sphere3: " << pl1sp3.intersects << ", Distance: " << pl1sp3.distance << endl;
    cout << "plane1 intersects sphere4: " << pl1sp4.intersects << ", Distance: " << pl1sp4.distance << endl
         << endl;
}