#include "StateEndRace.h"
#include <iostream>

using namespace std;

StateEndRace::StateEndRace(){
    // constructor
    std::cout << "Estado EndRace Creado" << std::endl;
    // Inicializamos las facadas
    renderEngine = RenderFacadeManager::GetInstance()->GetRenderFacade();
    //renderFacadeManager->InitializeIrrlicht();

    renderEngine->FacadeInitEndRace();

}

void StateEndRace::InitState() {

    cout << "~~~ ENTRO A ENDRACE" << endl;

    if (!soundEngine){
        soundEngine = SoundFacadeManager::GetInstance()->GetSoundFacade();
        cout << "~~~ SoundEngine en ENDRACE es -> " << soundEngine << endl;
    }
    soundEngine->SetState(5);
    soundEngine->PlayEvent3D("Musica/fin_partida");
}


void StateEndRace::Render(){
    renderEngine->FacadeDrawEndRace();
}


void StateEndRace::Input(){
    renderEngine->FacadeCheckInputEndRace();
    InputFacadeManager::GetInstance()->GetInputFacade()->CheckInputEndRace();
}


void StateEndRace::Update(){
}