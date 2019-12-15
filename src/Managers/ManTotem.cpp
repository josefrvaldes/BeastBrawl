#include "ManTotem.h"
#include "../Entities/Totem.h"
#include "../EventManager/Event.h"
#include "../EventManager/EventManager.h"
#include "../Facade/Render/RenderFacadeManager.h"

class Position;
using namespace std;

ManTotem::ManTotem() {
    SubscribeToEvents();
    CreateTotem();
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


void ManTotem::AppertainCar(DataMap d){
    std::cout << "El totem pasa a ser de un coche y ya no esta en el mapa" << std::endl;
    shared_ptr<RenderFacadeManager> renderFacadeManager = RenderFacadeManager::GetInstance();
    shared_ptr<RenderFacade> renderEngine = renderFacadeManager->GetRenderFacade();
    for(long unsigned int i=0; i< totems.size(); ++i){
        std::cout << "Falla aqui ??" << std::endl;
        if(totems[i] == any_cast<shared_ptr<Entity>>(d["Totem"])){
            renderEngine->DeleteEntity(totems[i].get());
            totems.erase(totems.begin()+i);
        }
        std::cout << "SI" << std::endl;
    }
}


void ManTotem::SubscribeToEvents() {
    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::COLLISION_TOTEM,
        bind(&ManTotem::AppertainCar, this, placeholders::_1),
        "AppertainCar"));
}