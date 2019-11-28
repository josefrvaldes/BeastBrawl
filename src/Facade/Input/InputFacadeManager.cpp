#include "InputFacadeManager.h"
InputFacadeManager* InputFacadeManager::instance = 0;
InputFacadeManager* InputFacadeManager::GetInstance(){
    if(instance==0){
        instance = new InputFacadeManager();
    }
    return instance;
}