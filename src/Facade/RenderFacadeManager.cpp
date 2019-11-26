#include "RenderFacadeManager.h"

RenderFacadeManager& RenderFacadeManager::GetInstance(){
    static RenderFacadeManager instance;
    return instance;
}