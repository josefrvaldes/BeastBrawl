#include "SoundFacadeManager.h"

SoundFacadeManager* SoundFacadeManager::instance = 0;

SoundFacadeManager* SoundFacadeManager::GetInstance(){
    if(instance==0){
        instance = new SoundFacadeManager();
    }
    return instance;
}