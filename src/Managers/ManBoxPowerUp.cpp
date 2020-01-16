#include "./ManBoxPowerUp.h"
//#include <functional> 
#include <iostream>
#include "../Entities/BoxPowerUp.h"
#include "../EventManager/Event.h"
#include "../EventManager/EventManager.h"

class Position;
using namespace std;

// se ejecuta en caso de que alguno de los coches coja la caja
void ManBoxPowerUp::EjecutarMeHanCogido(DataMap d) {
    auto cBoxPowerUp = any_cast<CBoxPowerUp*>(d["BoxPowerUpComp"]);
    auto actualBox   = any_cast<shared_ptr<Entity>>(d["actualBox"]);

    if(cBoxPowerUp->active == true){
        //cout << "Han cogido un powerup, madafaka!! sera la primera" << endl;
        auto renderFacadeManager = RenderFacadeManager::GetInstance();
        auto renderEngine = renderFacadeManager->GetRenderFacade();
        renderEngine->DeleteEntity(actualBox.get());       // se elmina la caja en irrlich para que no la dibuje, pero en nuestro array sigue estando
        cBoxPowerUp->active = false;
        cBoxPowerUp->timeStart = system_clock::now();

        DataMap data;
        auto cTranformableBox = static_cast<CTransformable*>(actualBox.get()->GetComponent(CompType::TransformableComp).get());
        data["posBox"] = cTranformableBox->position;
        EventManager::GetInstance().AddEventMulti(Event{EventType::BREAK_BOX, data});
    }
}


ManBoxPowerUp::ManBoxPowerUp() {
    SubscribeToEvents();
    cout << "Hemos creado el manager de powerup, ahora tenemos " << BoxPowerUps.size() << " powerups" << endl;
}


ManBoxPowerUp::~ManBoxPowerUp() {
    cout << "Llamando al destructor de ManBoxPowerUps" << endl;
    BoxPowerUps.clear();
    BoxPowerUps.shrink_to_fit();
}


void ManBoxPowerUp::CreateBoxPowerUp(glm::vec3 _position){
	shared_ptr<BoxPowerUp> p = make_shared<BoxPowerUp>(_position);
    BoxPowerUps.push_back(p);
}


void ManBoxPowerUp::CreateBoxPowerUp() {
    shared_ptr<BoxPowerUp> p = make_shared<BoxPowerUp>();
    BoxPowerUps.push_back(p);
}


void ManBoxPowerUp::SubscribeToEvents() {
    EventManager::GetInstance().SuscribeMulti(Listener(
        EventType::CATCH_BOX_POWERUP,
        bind(&ManBoxPowerUp::EjecutarMeHanCogido, this, placeholders::_1),
        "EjecutarMeHanCogido"));
    
    EventManager::GetInstance().SuscribeMulti(Listener(
        EventType::CATCH_AI_BOX_POWERUP,
        bind(&ManBoxPowerUp::EjecutarMeHanCogido, this, placeholders::_1),
        "EjecutarMeHanCogido"));

    EventManager::GetInstance().SuscribeMulti(Listener(
        EventType::CATCH_BOX_WITH_POWERUP,
        bind(&ManBoxPowerUp::EjecutarMeHanCogido, this, placeholders::_1),
        "EjecutarMeHanCogido"));
}


// se crea la caja de irrlich eliminada anteriormente
void ManBoxPowerUp::resetBox(BoxPowerUp* resetBox){
    auto renderFacadeManager = RenderFacadeManager::GetInstance();
    auto renderEngine = renderFacadeManager->GetRenderFacade();
    renderEngine->FacadeAddObject(resetBox);
}
