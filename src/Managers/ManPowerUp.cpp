#include "./ManPowerUp.h"
#include <iostream>
#include "../Entities/PowerUp.h"
#include "../EventManager/Event.h"
#include "../EventManager/EventManager.h"

class Position;
using namespace std;

void EjecutarMeHanCogido(Data d) {
    cout << "Han cogido un powerup, madafaka!!" << endl;
}

ManPowerUp::ManPowerUp() {
    SubscribeToEvents();
    CreatePowerUp(30, 30, 30);
    cout << "Hemos creado un powerup, ahora tenemos " << entities.size() << " powerups" << endl;
}

ManPowerUp::~ManPowerUp() {
    cout << "Llamando al destructor de manpowerup" << endl;
}

void ManPowerUp::CreatePowerUp(float x, float y, float z) {
    shared_ptr<PowerUp> p = make_shared<PowerUp>(x, y, z);
    entities.push_back(p);
}

void ManPowerUp::CreatePowerUp() {
    shared_ptr<PowerUp> p = make_shared<PowerUp>();
    entities.push_back(p);
}

void ManPowerUp::SubscribeToEvents() {
    shared_ptr<EventManager> em = EventManager::GetInstance();
    em->Suscribe(Listener{EventType::POWER_UP_COGIDO, EjecutarMeHanCogido, "EjecutarMeHanCogido"});
}

//eventManager->TriggerEvent(EventType::HAN_COGIDO_A_ALGUIEN, id_mio, id_que_he_cogido);
