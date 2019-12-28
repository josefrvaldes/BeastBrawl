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

    soundEngine = SoundFacadeManager::GetInstance()->GetSoundFacade();
    if (soundEngine){
        soundEngine->SetState(5);
    }
}


void StateEndRace::Render(){
    renderEngine->FacadeDrawEndRace();
}


void StateEndRace::Input(){
    renderEngine->FacadeCheckInputEndRace();
}


void StateEndRace::Update(){
}