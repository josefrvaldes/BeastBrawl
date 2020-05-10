#include "StateEndTournament.h"
#include <iostream>

using namespace std;

StateEndTournament::StateEndTournament(){

    std::cout << "> ENDTOURNAMENT constructor" << std::endl;

    // Inicializamos las facadas
    renderEngine = RenderFacadeManager::GetInstance()->GetRenderFacade();
    inputEngine = InputFacadeManager::GetInstance()->GetInputFacade();
    //renderFacadeManager->InitializeIrrlicht();

    renderEngine->FacadeInitEndTournament();
    InitState();

}

void StateEndTournament::InitState() {

    if (!soundEngine){
        soundEngine = SoundFacadeManager::GetInstance()->GetSoundFacade();
    }
    soundEngine->SetState(5);
}


void StateEndTournament::Render(){
    renderEngine->FacadeBeginScene();
    renderEngine->FacadeDrawEndTournament();
    renderEngine->FacadeEndScene();
}


void StateEndTournament::Input(){
    renderEngine->FacadeCheckInputEndTournament();
    //inputEngine->CheckInputEndTournament();
}


void StateEndTournament::Update(){
    EventManager::GetInstance().Update();

}