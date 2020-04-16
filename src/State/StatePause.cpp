#include "StatePause.h"
#include <iostream>

using namespace std;

StatePause::StatePause(){
    // constructor
    std::cout << "> PAUSE constructor" << std::endl;
    // Inicializamos las facadas
    renderEngine = RenderFacadeManager::GetInstance()->GetRenderFacade();
    //inputEngine = InputFacadeManager::GetInstance()->GetInputFacade();
    //renderFacadeManager->InitializeIrrlicht();

    renderEngine->FacadeInitPause();

    InitState();

}

void StatePause::InitState() {
    if (!soundEngine) {
        soundEngine = SoundFacadeManager::GetInstance()->GetSoundFacade();
    }
    soundEngine->SetState(7);
}


void StatePause::Render(){
    renderEngine->FacadeBeginScene();
    renderEngine->FacadeDrawPause();
    renderEngine->FacadeEndScene();
}


void StatePause::Input(){
    renderEngine->FacadeCheckInputPause();
}


void StatePause::Update(){
    //EventManager::GetInstance().Update();

}