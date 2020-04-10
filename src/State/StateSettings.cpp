#include "StateSettings.h"

#include <iostream>

StateSettings::StateSettings(){

    renderEngine = RenderFacadeManager::GetInstance()->GetRenderFacade();
    renderEngine->FacadeInitSettings();

    InitState();
    std::cout << ">>>>> Estado GAME OPTIONS creado" << std::endl;

}

void StateSettings::InitState() {

    /*cout << "~~~ ENTRO A MENU" << endl;
    if (!soundEngine){
        soundEngine = SoundFacadeManager::GetInstance()->GetSoundFacade();
        cout << "~~~ SoundEngine en MENU es -> " << soundEngine << endl;
    }
    soundEngine->SetState(2);
    soundEngine->PlayEvent("Musica/menu");*/

    //cout << "> MENU iniciado" << endl;
}


void StateSettings::Render(){
    renderEngine->FacadeBeginScene();
    renderEngine->FacadeDrawSettings();
    renderEngine->FacadeEndScene();
}


void StateSettings::Input(){
    renderEngine->FacadeCheckInputSettings();
}


void StateSettings::Update(){
    EventManager::GetInstance().Update();
}