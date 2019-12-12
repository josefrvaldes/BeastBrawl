#include "InputFacadeManager.h"

const shared_ptr<InputFacadeManager> InputFacadeManager::instance = make_shared<InputFacadeManager>();
shared_ptr<InputFacadeManager> InputFacadeManager::GetInstance() {
    //static EventManager instance;
    // if(instance==nullptr){
    //     instance = make_shared<EventManager>();
    // }
    return instance;
}
