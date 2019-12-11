#include "ManCar.h"
#include <functional>
#include <iostream>
#include "../Entities/Camera.h"
#include "../Entities/Car.h"
#include "../EventManager/Event.h"
#include "../EventManager/EventManager.h"
#include "../Systems/Physics.h"
#include "../Components/CPowerUp.h"

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

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::PRESS_C,
        bind(&ManCar::CatchPowerUp, this, placeholders::_1),
        "CatchPowerUp"));

}

void ManCar::CatchPowerUp(Data d) {
    // cout << "Han llamado izquierda" << endl;
    //physics->TurnLeft(car.get(), cam);
    int indx = rand() % 6+1;
    auto cPowerUpCar = static_cast<CPowerUp*>(car.get()->GetComponent(CompType::PowerUpComp).get());
    if(cPowerUpCar->typePowerUp == typeCPowerUp::None){
        cPowerUpCar->typePowerUp = (typeCPowerUp)indx;
        std::cout << "Mi super powerUp es:   " << (int)cPowerUpCar->typePowerUp << std::endl;
    }
    //cPowerUp->typePowerUp = dynamic_cast<typeCPowerUp*>(indx);
}

void ManCar::TurnLeftCar(Data d) {
    // cout << "Han llamado izquierda" << endl;
    physics->TurnLeft(car.get(), cam);
}

void ManCar::NotTurning(Data d) {
    // cout << "Han llamado not turning" << endl;
    physics->NotTurning(car.get(), cam);
}

void ManCar::Decelerate(Data d) {
    // cout << "Han llamado decel" << endl;
    physics->Decelerate(car.get(), cam);
}

void ManCar::NotAcceleratingOrDecelerating(Data d) {
    // cout << "Han llamado notaccel" << endl;
    physics->NotAcceleratingOrDecelerating(car.get(), cam);
}

void ManCar::TurnRightCar(Data d) {
    // cout << "Han llamado derecha" << endl;
    physics->TurnRight(car.get(), cam);
}

void ManCar::AccelerateCar(Data d) {
    // cout << "Han llamado acelerar" << endl;
    physics->Accelerate(car.get(), cam);
}
