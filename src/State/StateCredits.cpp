#include "StateCredits.h"
#include <iostream>

StateCredits::StateCredits(){

    std::cout << "> CREDITS constructor" << std::endl;

    renderEngine = RenderFacadeManager::GetInstance()->GetRenderFacade();
    renderEngine->FacadeInitCredits();

}

// Cargamos los bancos de sonido Menu.
void StateCredits::InitState() {

    /*cout << "~~~ ENTRO A MENU" << endl;
    if (!soundEngine){
        soundEngine = SoundFacadeManager::GetInstance()->GetSoundFacade();
        cout << "~~~ SoundEngine en MENU es -> " << soundEngine << endl;
    }
    soundEngine->SetState(2);
    soundEngine->PlayEvent("Musica/menu");*/

    //cout << "> MENU iniciado" << endl;
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