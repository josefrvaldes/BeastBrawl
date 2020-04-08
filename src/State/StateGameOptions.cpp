#include "StateGameOptions.h"

#include <iostream>

StateGameOptions::StateGameOptions(){

    renderEngine = RenderFacadeManager::GetInstance()->GetRenderFacade();
    renderEngine->FacadeInitGameOptions();

    InitState();
    std::cout << ">>>>> Estado GAME OPTIONS creado" << std::endl;

}

void StateGameOptions::InitState() {

    /*cout << "~~~ ENTRO A MENU" << endl;
    if (!soundEngine){
        soundEngine = SoundFacadeManager::GetInstance()->GetSoundFacade();
        cout << "~~~ SoundEngine en MENU es -> " << soundEngine << endl;
    }
    soundEngine->SetState(2);
    soundEngine->PlayEvent("Musica/menu");*/

    //cout << "> MENU iniciado" << endl;
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