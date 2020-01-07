#include "ManBoundingWall.h"
#include <functional>
#include <iostream>
#include "../Entities/BoundingWall.h"
#include "Manager.h"

using namespace std;

ManBoundingWall::ManBoundingWall() {
    SubscribeToEvents();
    // el primer bool marca la orientación: si es true está orientado en el eje X, si es false, está orientado en el eje Z
    // el segundo bool marca el sentido del vector normal
    shared_ptr<BoundingWall> p1 = make_shared<BoundingWall>(vec3(-20.f, 20.f, -460.f), true, false);
    shared_ptr<BoundingWall> p2 = make_shared<BoundingWall>(vec3(-20.f, 20.f, 760.f), true, true); // orientation
    shared_ptr<BoundingWall> p3 = make_shared<BoundingWall>(vec3(-470.f, 20.f, 200.f), false, false);
    shared_ptr<BoundingWall> p4 = make_shared<BoundingWall>(vec3(480.f, 20.f, 200.f), false, true);
    entities.push_back(p1);
    entities.push_back(p2);
    entities.push_back(p3);
    entities.push_back(p4);
    cout << "Hemos creado un bounding wall, ahora tenemos " << entities.size() << " bounding walls" << endl;
}

ManBoundingWall::~ManBoundingWall() {
    cout << "Llamando al destructor de ManBoundingWall" << endl;
}

void ManBoundingWall::SubscribeToEvents() {
    // EventManager::GetInstance()->SuscribeMulti(Listener(
    //     EventType::PRESS_I,
    //     bind(&ManCar::AccelerateCar, this, placeholders::_1),
    //     "AccelerateCar"));
}

void ManBoundingWall::Integrate(float delta) {
    //physics->update(GetCar().get(), cam.get());
}