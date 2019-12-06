#include "RenderFacadeManager.h"

const shared_ptr<RenderFacadeManager> RenderFacadeManager::instance = make_shared<RenderFacadeManager>();
shared_ptr<RenderFacadeManager> RenderFacadeManager::GetInstance() {
    //static EventManager instance;
    // if(instance==nullptr){
    //     instance = make_shared<EventManager>();
    // }
    return instance;
}
