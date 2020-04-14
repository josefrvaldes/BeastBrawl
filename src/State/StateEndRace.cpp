#include "StateEndRace.h"
#include <iostream>

using namespace std;

StateEndRace::StateEndRace(){
    // constructor
    //std::cout << ">>>>> Estado EndRace Creado" << std::endl;
    // Inicializamos las facadas
    renderEngine = RenderFacadeManager::GetInstance()->GetRenderFacade();
    inputEngine = InputFacadeManager::GetInstance()->GetInputFacade();
    //renderFacadeManager->InitializeIrrlicht();

    renderEngine->FacadeInitEndRace();
    cout << "QUIERO HACER INIT STATE DE END GAME" << endl;
    InitState();

}

void StateEndRace::InitState() {

    //cout << "~~~ ENTRO A ENDRACE" << endl;

    if (!soundEngine){
        soundEngine = SoundFacadeManager::GetInstance()->GetSoundFacade();
    }
    soundEngine->SetState(5);
    soundEngine->PlayEvent("Musica/fin_partida");
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