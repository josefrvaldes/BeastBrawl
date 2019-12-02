#include "PhysicsFacadeManager.h"
PhysicsFacadeManager* PhysicsFacadeManager::instance=0;
PhysicsFacadeManager* PhysicsFacadeManager::GetInstance(){
    if(instance==0){
        instance = new PhysicsFacadeManager();
    }
    return instance;
}