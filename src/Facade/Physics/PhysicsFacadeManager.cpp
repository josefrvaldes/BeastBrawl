#include "PhysicsFacadeManager.h"

const shared_ptr<PhysicsFacadeManager> PhysicsFacadeManager::instance = make_shared<PhysicsFacadeManager>();
shared_ptr<PhysicsFacadeManager> PhysicsFacadeManager::GetInstance() {
    //static EventManager instance;
    // if(instance==nullptr){
    //     instance = make_shared<EventManager>();
    // }
    return instance;
}
