#include <iostream>
#include <functional>
#include "ManCar.h"
#include "../Entities/Car.h"
#include "../EventManager/Event.h"
#include "../EventManager/EventManager.h"

class Position;
using namespace std;

ManCar::ManCar() {
    SubscribeToEvents();
    CreateMainCar();
    cout << "Hemos creado un powerup, ahora tenemos " << entities.size() << " powerups" << endl;
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
    Listener listenerAcelerarCoche = Listener(
        EventType::PRESS_A, 
        bind(&ManCar::AcelerarCoche, this, placeholders::_1),
        "EjecutarMeHanCogido");
    EventManager::GetInstance()->SuscribeMulti(listenerAcelerarCoche);
}

void ManCar::AcelerarCoche(Data d) {
    cout << "Se ha llamado al evento acelerar coche" << endl;
}

//eventManager->TriggerEvent(EventType::HAN_COGIDO_A_ALGUIEN, id_mio, id_que_he_cogido);
