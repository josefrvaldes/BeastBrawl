#include "ManBoundingWall.h"
#include <functional>
#include <iostream>
#include "../Entities/BoundingWall.h"
#include "Manager.h"

using namespace std;

ManBoundingWall::ManBoundingWall() {
    SubscribeToEvents();
    shared_ptr<BoundingWall> b = make_shared<BoundingWall>();
    entities.push_back(b);
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