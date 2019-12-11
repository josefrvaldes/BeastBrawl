#include "./ManBoxPowerUp.h"
//#include <functional>
#include <iostream>
#include "../Entities/BoxPowerUp.h"
#include "../EventManager/Event.h"
#include "../EventManager/EventManager.h"

class Position;
using namespace std;

void ManBoxPowerUp::EjecutarMeHanCogido(Data d) {
    if(BoxPowerUps[0].get()->active == true){
        cout << "Han cogido un powerup, madafaka!! sera la primera" << endl;
        //shared_ptr<RenderFacade> renderFacadeManager;
        shared_ptr<RenderFacadeManager> renderFacadeManager = RenderFacadeManager::GetInstance();
        shared_ptr<RenderFacade> renderEngine = renderFacadeManager->GetRenderFacade();
        renderEngine->DeleteEntity(BoxPowerUps[0].get());

        //debemos, de forma aleatoria asignarle un tipo al powerUp
        BoxPowerUps[0].get()->active = false;
        BoxPowerUps[0].get()->timeStart = system_clock::now();
    }
}

ManBoxPowerUp::ManBoxPowerUp() {
    SubscribeToEvents();
    //CreatePowerUp(glm::vec3(30.0, 30.0, 30.0));
    cout << "Hemos creado el manager de powerup, ahora tenemos " << BoxPowerUps.size() << " powerups" << endl;
}

ManBoxPowerUp::~ManBoxPowerUp() {
    cout << "Llamando al destructor de manpowerup" << endl;
    BoxPowerUps.clear();
    BoxPowerUps.shrink_to_fit();
}


void ManBoxPowerUp::CreateBoxPowerUp(glm::vec3 _position) 
{
	shared_ptr<BoxPowerUp> p = make_shared<BoxPowerUp>(_position);
    BoxPowerUps.push_back(p);
}

void ManBoxPowerUp::CreateBoxPowerUp() {
    shared_ptr<BoxPowerUp> p = make_shared<BoxPowerUp>();
    BoxPowerUps.push_back(p);
}

void ManBoxPowerUp::SubscribeToEvents() {
    //EventManager::GetInstance()->SuscribeMulti(Listener{EventType::PRESS_C, EjecutarMeHanCogido, "EjecutarMeHanCogido"});

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::PRESS_C,
        bind(&ManBoxPowerUp::EjecutarMeHanCogido, this, placeholders::_1),
        "EjecutarMeHanCogido"));
}

void ManBoxPowerUp::resetBox(BoxPowerUp* resetBox){
    shared_ptr<RenderFacadeManager> renderFacadeManager = RenderFacadeManager::GetInstance();
    shared_ptr<RenderFacade> renderEngine = renderFacadeManager->GetRenderFacade();
    renderEngine->FacadeAddObject(resetBox);
}

//eventManager->TriggerEvent(EventType::HAN_COGIDO_A_ALGUIEN, id_mio, id_que_he_cogido);
