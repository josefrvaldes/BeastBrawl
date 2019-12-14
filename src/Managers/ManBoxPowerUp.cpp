#include "./ManBoxPowerUp.h"
//#include <functional>
#include <iostream>
#include "../Entities/BoxPowerUp.h"
#include "../EventManager/Event.h"
#include "../EventManager/EventManager.h"

class Position;
using namespace std;

void ManBoxPowerUp::EjecutarMeHanCogido(DataMap d) {
    /*uint32_t contador = 0;
    uint32_t posVector = 0;
    for(auto Box : BoxPowerUps){
        auto cBoxId = static_cast<CId*>(Box.get()->GetComponent(CompType::IdComp).get());
        if(cBoxId->id == d.id){
            posVector = contador;
        }
        contador++;
    }*/
    auto cBoxPowerUp = any_cast<CBoxPowerUp*>(d["BoxPowerUpComp"]);
    auto actualBox   = any_cast<shared_ptr<Entity>>(d["actualBox"]);
    //auto cBoxPowerUp2 = static_cast<CBoxPowerUp*>(BoxPowerUps[0].get()->GetComponent(CompType::BoxPowerUpComp).get());
    if(cBoxPowerUp->active == true){
        cout << "Han cogido un powerup, madafaka!! sera la primera" << endl;
        //shared_ptr<RenderFacade> renderFacadeManager;
        shared_ptr<RenderFacadeManager> renderFacadeManager = RenderFacadeManager::GetInstance();
        shared_ptr<RenderFacade> renderEngine = renderFacadeManager->GetRenderFacade();
        renderEngine->DeleteEntity(actualBox.get());       // se elmina la caja en irrlich para que no la dibuje, pero en nuestro array sigue estando
        //debemos, de forma aleatoria asignarle un tipo al powerUp
        cBoxPowerUp->active = false;
        cBoxPowerUp->timeStart = system_clock::now();
    }
}

ManBoxPowerUp::ManBoxPowerUp() {
    SubscribeToEvents();
    //CreatePowerUp(glm::vec3(30.0, 30.0, 30.0));
    cout << "Hemos creado el manager de powerup, ahora tenemos " << BoxPowerUps.size() << " powerups" << endl;
}

ManBoxPowerUp::~ManBoxPowerUp() {
    cout << "Llamando al destructor de ManBoxPowerUps" << endl;
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
    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::CATCH_BOX_POWERUP,
        bind(&ManBoxPowerUp::EjecutarMeHanCogido, this, placeholders::_1),
        "EjecutarMeHanCogido"));
}

// se crea la caja de irrlich eliminada anteriormente
void ManBoxPowerUp::resetBox(BoxPowerUp* resetBox){
    shared_ptr<RenderFacadeManager> renderFacadeManager = RenderFacadeManager::GetInstance();
    shared_ptr<RenderFacade> renderEngine = renderFacadeManager->GetRenderFacade();
    renderEngine->FacadeAddObject(resetBox);
}

//eventManager->TriggerEvent(EventType::HAN_COGIDO_A_ALGUIEN, id_mio, id_que_he_cogido);
