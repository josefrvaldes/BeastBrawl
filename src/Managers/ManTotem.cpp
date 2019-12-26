#include "ManTotem.h"
#include "../Entities/Totem.h"
#include "../EventManager/Event.h"
#include "../EventManager/EventManager.h"
#include "../Facade/Render/RenderFacadeManager.h"
#include "../Components/CTransformable.h"

class Position;
using namespace std;

ManTotem::ManTotem() {
    SubscribeToEvents();
   // CreateTotem();
}

ManTotem::~ManTotem() {
    cout << "Llamando al destructor de ManTotem" << endl;
    totems.clear();
    totems.shrink_to_fit();
}


void ManTotem::CreateTotem() {
    if(totems.size() == 0){
        shared_ptr<Totem> totem = make_shared<Totem>();
        totems.push_back(totem);
    }
}

void ManTotem::CreateTotem(glm::vec3 _position) {
    if(totems.size() == 0){
        shared_ptr<Totem> totem = make_shared<Totem>(_position);
        totems.push_back(totem);
    }
}


// elimina el totem de irrlich y lo oculta del mapa
void ManTotem::AppertainCar(DataMap d){
    auto renderFacadeManager = RenderFacadeManager::GetInstance();
    auto renderEngine = renderFacadeManager->GetRenderFacade();
    for(long unsigned int i=0; i< totems.size(); ++i){
        if(totems[i] == any_cast<shared_ptr<Entity>>(d["Totem"])){
            renderEngine->DeleteEntity(totems[i].get());
            totems.erase(totems.begin()+i);
        }
    }
}


// crea el objeto en irlich y lo pone en el mapa de nuevo
void ManTotem::ResetTotem(DataMap d){
    auto transfActualCar = any_cast<CTransformable*>(d["TransfCarPos"]);
    glm::vec3 posNewTotem = glm::vec3(0.0f,20.0f,0.0f);
    float angleRotation = (transfActualCar->rotation.y * 3.141592) / 180.0;
    posNewTotem.x = transfActualCar->position.x - cos(angleRotation)*(-25);
    posNewTotem.z = transfActualCar->position.z + sin(angleRotation)*(-25);

    CreateTotem(posNewTotem);

    // Debemos de crearlo tambien en iirlicht
    auto renderFacadeManager = RenderFacadeManager::GetInstance();
    auto renderEngine = renderFacadeManager->GetRenderFacade();
    //for(long unsigned int i=0; i< totems.size(); ++i){
        renderEngine->FacadeAddObject(totems[0].get());
   // }
}


void ManTotem::SubscribeToEvents() {
    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::COLLISION_PLAYER_TOTEM,
        bind(&ManTotem::AppertainCar, this, placeholders::_1),
        "AppertainCar"));
    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::COLLISION_AI_TOTEM,
        bind(&ManTotem::AppertainCar, this, placeholders::_1),
        "AppertainCar"));
    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::DROP_TOTEM,
        bind(&ManTotem::ResetTotem, this, placeholders::_1),
        "ResetTotem"));
}