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
    CreatePowerUp(glm::vec3(30.0, 30.0, 30.0));
    cout << "Hemos creado un powerup, ahora tenemos " << entities.size() << " powerups" << endl;
}

ManPowerUp::~ManPowerUp() {
    cout << "Llamando al destructor de manpowerup" << endl;
}


void ManPowerUp::CreatePowerUp(glm::vec3 _position) 
{
	shared_ptr<PowerUp> p = make_shared<PowerUp>(_position);
    entities.push_back(p);
}

void ManPowerUp::CreatePowerUp() {
    shared_ptr<PowerUp> p = make_shared<PowerUp>();
    entities.push_back(p);
}

void ManPowerUp::SubscribeToEvents() {
    EventManager::GetInstance()->SuscribeMulti(Listener{EventType::POWER_UP_COGIDO, EjecutarMeHanCogido, "EjecutarMeHanCogido"});
}

//eventManager->TriggerEvent(EventType::HAN_COGIDO_A_ALGUIEN, id_mio, id_que_he_cogido);
