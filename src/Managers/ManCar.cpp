#include "ManCar.h"
#include <functional>
#include <iostream>
#include "../Entities/Camera.h"
#include "../Entities/Car.h"
#include "../EventManager/Event.h"
#include "../EventManager/EventManager.h"
#include "../Systems/Physics.h"

class Position;
using namespace std;

ManCar::ManCar() {
    SubscribeToEvents();
    CreateMainCar();
    cout << "Hemos creado un powerup, ahora tenemos " << entities.size() << " powerups" << endl;
}

// TODO: este paso de physics es kk, hay que revisarlo de enviarlo como referencia o algo pero me da error
ManCar::ManCar(Physics *_physics, Camera *_cam) : ManCar() {
    this->physics = _physics;
    cout << "Tenemos en ManCar un physics con dir de memoria " << physics << endl;
    this->cam = _cam;
}

ManCar::~ManCar() {
    cout << "Llamando al destructor de manpowerup" << endl;
}

void ManCar::CreateMainCar() {
    car = make_shared<Car>();
}

void ManCar::CreateCar() {
    shared_ptr<Car> p = make_shared<Car>();
    entities.push_back(p);
}

void ManCar::SubscribeToEvents() {
    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::PRESS_I,
        bind(&ManCar::AccelerateCar, this, placeholders::_1),
        "AccelerateCar"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::PRESS_O,
        bind(&ManCar::Decelerate, this, placeholders::_1),
        "Decelerate"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::PRESS_A,
        bind(&ManCar::TurnLeftCar, this, placeholders::_1),
        "TurnLeftCar"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::PRESS_D,
        bind(&ManCar::TurnRightCar, this, placeholders::_1),
        "TurnRightCar"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::NO_I_O_PRESS,
        bind(&ManCar::NotAcceleratingOrDecelerating, this, placeholders::_1),
        "NotAcceleratingOrDecelerating"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::NO_A_D_PRESS,
        bind(&ManCar::NotTurning, this, placeholders::_1),
        "NotTurning"));
}

void ManCar::TurnLeftCar(Data d) {
    physics->TurnLeft(car.get(), cam);
}

void ManCar::NotTurning(Data d) {
    physics->NotTurning(car.get(), cam);
}

void ManCar::Decelerate(Data d) {
    physics->Decelerate(car.get(), cam);
}

void ManCar::NotAcceleratingOrDecelerating(Data d) {
    physics->NotAcceleratingOrDecelerating(car.get(), cam);
}

void ManCar::TurnRightCar(Data d) {
    physics->TurnRight(car.get(), cam);
}

void ManCar::AccelerateCar(Data d) {
    // cout << "Se ha llamado al evento acelerar coche" << endl;
    // cout << "Tenemos en AccelerateCar un physics con dir de memoria " << physics << endl;

    // auto components = car.get()->GetComponents();
    // auto cTransformable = static_cast<CTransformable *>(components[CompType::TransformableComp].get());

    // auto componentsCam = cam->GetComponents();
    // auto cTransformableCam = static_cast<CTransformable *>(componentsCam[CompType::TransformableComp].get());

    // cout << "La pos del coche antes es " << cTransformable->position.x << "," << cTransformable->position.y << "," <<  cTransformable->position.z << endl;
    // cout << "La pos de la cámara antes es " << cTransformableCam->position.x << "," << cTransformableCam->position.y << "," <<  cTransformableCam->position.z << endl;
    physics->Accelerate(car.get(), cam);
    // cout << "La pos del coche después es " << cTransformable->position.x << "," << cTransformable->position.y << "," <<  cTransformable->position.z << endl;
    // cout << "La pos de la cámara después es " << cTransformableCam->position.x << "," << cTransformableCam->position.y << "," <<  cTransformableCam->position.z << endl;
}
