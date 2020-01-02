#include "./ManPowerUp.h"
#include <iostream>
#include "../Entities/PowerUp.h"
#include "../EventManager/Event.h"
#include "../EventManager/EventManager.h"
#include "../Aliases.h"
#include "../Facade/Render/RenderFacadeManager.h"

class Position;
using namespace std;


ManPowerUp::ManPowerUp() {
    SubscribeToEvents();
    //CreatePowerUp(glm::vec3(30.0, 30.0, 30.0));
    cout << "Hemos creado el manager de powerup, ahora tenemos " << endl;
}


ManPowerUp::~ManPowerUp() {
    cout << "Llamando al destructor de ManPowerUps" << endl;
    PowerUps.clear();
    PowerUps.shrink_to_fit();
}


void ManPowerUp::CreatePowerUp(DataMap d) {

    typeCPowerUp type = any_cast<typeCPowerUp>(d["typePowerUp"]);
    CTransformable *transforSalida = any_cast<CTransformable *>(d["posCocheSalida"]);
    CTransformable *transforPerse = any_cast<CTransformable *>(d["posCochePerseguir"]);
    shared_ptr<PowerUp> powerUp = make_shared<PowerUp>(transforSalida->position, transforSalida->rotation, type, transforPerse);
    PowerUps.push_back(powerUp);

    auto renderFacadeManager = RenderFacadeManager::GetInstance();
    auto renderEngine = renderFacadeManager->GetRenderFacade();
    renderEngine->FacadeAddObject(powerUp.get());

    //Cuando creamos el powerUp, ponemos su tiempo inicial de inactivadad --> para no danyarnos a nostros mismos
    static_cast<CPowerUp*>(powerUp.get()->GetComponent(CompType::PowerUpComp).get())->timeStart = system_clock::now();
}


// TO-DO ELIMINARLO TODO AL MISMO TIEMPO ANTES DE RENDERIZAR 
void ManPowerUp::DeletePowerUp(DataMap d){
    auto renderFacadeManager = RenderFacadeManager::GetInstance();
    auto renderEngine = renderFacadeManager->GetRenderFacade();
    for(long unsigned int i=0; i< PowerUps.size(); ++i){
        if(PowerUps[i] == any_cast<shared_ptr<Entity>>(d["PowerUp"])){
            renderEngine->DeleteEntity(PowerUps[i].get());
            PowerUps.erase(PowerUps.begin()+i);
        }
    }
}


// TO-DO : tener una variable de control para eliminar todas las cosas de los arrays a la vez CUIDADO CON ESOOOO
void ManPowerUp::SubscribeToEvents() {
    // lo ejecuta el coche al tirar power up
    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::PowerUp_Create,
        bind(&ManPowerUp::CreatePowerUp, this, placeholders::_1),
        "CreatePowerUp"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::COLLISION_ENTITY_POWERUP,
        bind(&ManPowerUp::DeletePowerUp, this, placeholders::_1),
        "DeletePowerUp"));
    
    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::COLLISION_ENTITY_AI_POWERUP,
        bind(&ManPowerUp::DeletePowerUp, this, placeholders::_1),
        "DeletePowerUp"));
}

