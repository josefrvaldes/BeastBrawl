#include "RenderFacadeManager.h"
RenderFacadeManager* RenderFacadeManager::instance = 0;
RenderFacadeManager* RenderFacadeManager::GetInstance(){
    if(instance==0){
        instance = new RenderFacadeManager();
    }
    return instance;
}