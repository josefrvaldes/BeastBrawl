#include "StateCredits.h"
#include <iostream>

StateCredits::StateCredits(){

    std::cout << "> CREDITS constructor" << std::endl;

    renderEngine = RenderFacadeManager::GetInstance()->GetRenderFacade();
    renderEngine->FacadeInitCredits();

}

// Cargamos los bancos de sonido Menu.
void StateCredits::InitState() {
    //if (!soundEngine){
    //    soundEngine = SoundFacadeManager::GetInstance()->GetSoundFacade();
    //}
    //soundEngine->SetState(9);
}


void StateCredits::Render(){
    renderEngine->FacadeBeginScene();
    renderEngine->FacadeDrawCredits();
    renderEngine->FacadeEndScene();
}


void StateCredits::Input(){
    renderEngine->FacadeCheckInputCredits();
}


void StateCredits::Update(){
    EventManager::GetInstance().Update();
}