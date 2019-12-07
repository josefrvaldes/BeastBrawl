#include "../Entities/PowerUp.h"
#include "../EventManager/EventManager.h"
#include "../EventManager/Event.h"
#include "./ManPowerUp.h"
#include <iostream>

class Position;
using namespace std;


ManPowerUp::ManPowerUp()
{
    SubscribeToEvents();
    //CreatePowerUp(30, 20, 30);
    cout << "Hemos creado un powerup, ahora tenemos " << powerUps.size() << " powerups" << endl;
}

ManPowerUp::~ManPowerUp()
{
    powerUps.clear();
    powerUps.shrink_to_fit();
}


void ManPowerUp::CreatePowerUp(glm::vec3 _position) 
{
	shared_ptr<PowerUp> p = make_shared<PowerUp>(_position);
    powerUps.push_back(p);
}


void ManPowerUp::CreatePowerUp() 
{
	shared_ptr<PowerUp> p = make_shared<PowerUp>();
    powerUps.push_back(p);
}


void ManPowerUp::SubscribeToEvents() 
{
    //shared_ptr<EventManager> em = EventManager::GetInstance();
    //em->Suscribe(Listener {EventType::POWER_UP_COGIDO, EjecutarMeHanCogido, "EjecutarMeHanCogido"});
}




//eventManager->TriggerEvent(EventType::HAN_COGIDO_A_ALGUIEN, id_mio, id_que_he_cogido);



