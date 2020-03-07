#include "StateControls.h"
#include <iostream>

using namespace std;

StateControls::StateControls(){

    std::cout << "> MENU constructor" << std::endl;

    renderEngine = RenderFacadeManager::GetInstance()->GetRenderFacade();
    renderEngine->FacadeInitControler();

}

// Cargamos los bancos de sonido Menu.
void StateControls::InitState() {
    
    cout << "~~~ ENTRO A MENU" << endl;
    if (!soundEngine){
        soundEngine = SoundFacadeManager::GetInstance()->GetSoundFacade();
        cout << "~~~ SoundEngine en MENU es -> " << soundEngine << endl;
    }
    soundEngine->SetState(2);
    soundEngine->PlayEvent("Musica/menu");

    //cout << "> MENU iniciado" << endl;
}


void StateControls::Render(){
    renderEngine->FacadeDrawControler();
}


void StateControls::Input(){
    renderEngine->FacadeCheckInputControler();
}


void StateControls::Update(){
    EventManager::GetInstance().Update();
}