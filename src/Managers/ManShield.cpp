#include "ManShield.h"

#include <EventManager/EventManager.h>
#include <EventManager/Event.h>
#include <Components/CParentNode.h>
#include <Components/CId.h>
ManShield::ManShield(){
    SubscribeToEvents();
}


void ManShield::CreateShield(uint16_t parentId, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale){
    shared_ptr<Shield> e = make_shared<Shield>(parentId, position, rotation, scale);
    entities.push_back(e);
}

ManShield::~ManShield(){}


void ManShield::SubscribeToEvents(){
    EventManager::GetInstance().SubscribeMulti(Listener{
                EventType::UPDATE_SHIELD_VISIBILITY,
                bind(&ManShield::UpdateShieldVisibility, this, placeholders::_1),
                "UpdateShieldVisibility"});
}

void ManShield::UpdateShieldVisibility(DataMap* d){
    auto idEntity = any_cast<uint16_t>((*d)[ID]);
    auto visibility = any_cast<bool>((*d)[TRUEFALSE]);

    for(auto shield : entities){
        if(shield->HasComponent(CompType::ParentNodeComp)){

            auto cParentNode = static_cast<CParentNode*>(shield->GetComponent(CompType::ParentNodeComp).get());
            if(cParentNode->idParentNode == idEntity){
                //Se debe activar este sistema de particulas
                shared_ptr<DataMap> data = make_shared<DataMap>();
                (*data)[ID] = static_cast<CId*>(shield.get()->GetComponent(CompType::IdComp).get())->id;
                (*data)[TRUEFALSE] = visibility;
                EventManager::GetInstance().AddEventMulti(Event{EventType::UPDATE_FACADE_VISIBILITY, data});
            }
        }
    }
}