#include "StateGameOptions.h"

#include <iostream>

StateGameOptions::StateGameOptions(){

    std::cout << "> GAME OPTIONS constructor" << std::endl;

    renderEngine = RenderFacadeManager::GetInstance()->GetRenderFacade();
    renderEngine->FacadeInitGameOptions();

    InitState();

}

void StateGameOptions::InitState() {

    if (!soundEngine){
        soundEngine = SoundFacadeManager::GetInstance()->GetSoundFacade();
    }
    soundEngine->SetState(3);
}


void StateGameOptions::Render(){
    renderEngine->FacadeBeginScene();
    renderEngine->FacadeDrawGameOptions();
    renderEngine->FacadeEndScene();
}


void StateGameOptions::Input(){
    renderEngine->FacadeCheckInputGameOptions();
}


void StateGameOptions::Update(){
    EventManager::GetInstance().Update();
}

StateGameOptions::~StateGameOptions(){
    renderEngine->FacadeReleaseOptions();
}