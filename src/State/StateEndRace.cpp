#include "StateEndRace.h"
#include <iostream>

using namespace std;

StateEndRace::StateEndRace(){

    std::cout << "> ENDRACE constructor" << std::endl;

    // Inicializamos las facadas
    renderEngine = RenderFacadeManager::GetInstance()->GetRenderFacade();
    inputEngine = InputFacadeManager::GetInstance()->GetInputFacade();
    //renderFacadeManager->InitializeIrrlicht();

    renderEngine->FacadeInitEndRace();
    InitState();

}

void StateEndRace::InitState() {

    if (!soundEngine){
        soundEngine = SoundFacadeManager::GetInstance()->GetSoundFacade();
    }
    soundEngine->SetState(5);
}


void StateEndRace::Render(){
    renderEngine->FacadeBeginScene();
    renderEngine->FacadeDrawEndRace();
    renderEngine->FacadeEndScene();
}


void StateEndRace::Input(){
    renderEngine->FacadeCheckInputEndRace();
    //inputEngine->CheckInputEndRace();
}


void StateEndRace::Update(){
    EventManager::GetInstance().Update();

}