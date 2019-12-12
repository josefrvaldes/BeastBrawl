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
    // auto accelerateCar = [&](Data d) {
    //     cout << "Estamos ejecutando lambda " << endl;
    //     /*Car *c = this->GetCar().get();
    //     auto components = c->GetComponents();
    //     CTransformable *trans = static_cast<CTransformable*>(components[CompType::TransformableComp].get()); 
    //     cout << "El coche está en  " << trans->position.x << "," << trans->position.y << "," << trans->position.z << endl;*/
    //     //physics->Accelerate(car.get(), cam);
    // };

    /*int (*accelerate) (Data) = [&](Data d) {
        physics->Accelerate(car.get(), cam);
        return 0;
    };*/
    //int (*func_pointer) (int) = [](int a) { return a; };

    //accelerateCar(0);


    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::PRESS_I,
        [&](DataMap d){this->AccelerateCar(d);},
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

void ManCar::TurnLeftCar(DataMap d) {
    // cout << "Han llamado izquierda" << endl;
    physics->TurnLeft(car.get(), cam);
}

void ManCar::NotTurning(DataMap d) {
    // cout << "Han llamado not turning" << endl;
    physics->NotTurning(car.get(), cam);
}

void ManCar::Decelerate(DataMap d) {
    // cout << "Han llamado decel" << endl;
    physics->Decelerate(car.get(), cam);
}

void ManCar::NotAcceleratingOrDecelerating(DataMap d) {
    // cout << "Han llamado notaccel" << endl;
    physics->NotAcceleratingOrDecelerating(car.get(), cam);
}

void ManCar::TurnRightCar(DataMap d) {
    // cout << "Han llamado derecha" << endl;
    physics->TurnRight(car.get(), cam);
}

void ManCar::AccelerateCar(DataMap d) {
    // cout << "Estamos en AccelerateCar" << endl;
    // int mint = any_cast<int>(d["int"]);
    // cout << "Hemos obtenido el int" << mint << endl;
    // float mfloat = any_cast<float>(d["float"]);
    // cout << "Hemos obtenido el float" << endl;
    // vector<int> mvect = any_cast<vector<int>>(d["vector"]);
    // cout << "Hemos recibido por evento un int=" << mint << " un float=" << mfloat << " y un vector de int con size=" << mvect.size() << endl;
    
    physics->Accelerate(car.get(), cam);
}
