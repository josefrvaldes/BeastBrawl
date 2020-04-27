#include "StateSettings.h"

#include <iostream>

StateSettings::StateSettings(){

    std::cout << "> SETTINGS constructor" << std::endl;
    renderEngine = RenderFacadeManager::GetInstance()->GetRenderFacade();
    renderEngine->FacadeSuscribeEventsSettings();
    renderEngine->FacadeInitSettings();

    InitState();

}

void StateSettings::InitState() {
    if (!soundEngine){
        soundEngine = SoundFacadeManager::GetInstance()->GetSoundFacade();
    }
    soundEngine->SetState(8);
}


void StateSettings::Render(){
    renderEngine->FacadeBeginScene();
    renderEngine->FacadeDrawSettings();
    renderEngine->FacadeEndScene();
}


void StateSettings::Input(){
    renderEngine->FacadeCheckInputSettings();
}


void StateSettings::Update(){
    EventManager::GetInstance().Update();
}