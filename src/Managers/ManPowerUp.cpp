#include "../Entities/PowerUp.h"
#include "../EventManager/EventManager.h"
#include "../EventManager/Event.h"
#include "./ManPowerUp.h"
#include <iostream>

class Position;
using namespace std;

void EjecutarMeHanCogido(Data d) 
{
    cout << "Han cogido un powerup, madafaka!!" << endl;
}



ManPowerUp::ManPowerUp()
{
    SubscribeToEvents();
    CreatePowerUp(30, 30, 30);
    cout << "Hemos creado un powerup, ahora tenemos " << powerUps.size() << " powerups" << endl;
}

ManPowerUp::~ManPowerUp()
{
    powerUps.clear();
    powerUps.shrink_to_fit();
}


void ManPowerUp::CreatePowerUp(float x, float y, float z) 
{
	PowerUp *p = new PowerUp(x, y, z);
    powerUps.push_back(p);
}


void ManPowerUp::CreatePowerUp() 
{
	PowerUp *p = new PowerUp();
    powerUps.push_back(p);
}


void ManPowerUp::SubscribeToEvents() 
{
    EventManager *em = EventManager::GetInstance();
    em->Suscribe(Listener {EventType::POWER_UP_COGIDO, EjecutarMeHanCogido, "EjecutarMeHanCogido"});
}




//eventManager->TriggerEvent(EventType::HAN_COGIDO_A_ALGUIEN, id_mio, id_que_he_cogido);



