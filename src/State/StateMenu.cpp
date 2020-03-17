#include "StateMenu.h"
#include <iostream>

using namespace std;

StateMenu::StateMenu(){

    std::cout << "> MENU constructor" << std::endl;

    renderEngine = RenderFacadeManager::GetInstance()->GetRenderFacade();
    renderEngine->FacadeInitMenu();

    InitState();

}

// Cargamos los bancos de sonido Menu.
void StateMenu::InitState() {
    
    cout << "~~~ ENTRO A MENU" << endl;
    if (!soundEngine){
        soundEngine = SoundFacadeManager::GetInstance()->GetSoundFacade();
        cout << "~~~ SoundEngine en MENU es -> " << soundEngine << endl;
    }
    soundEngine->SetState(2);
    soundEngine->PlayEvent("Musica/menu");

    //cout << "> MENU iniciado" << endl;
}


void StateMenu::Render(){
    renderEngine->FacadeDrawMenu();
}


void StateMenu::Input(){
    renderEngine->FacadeCheckInputMenu();
}


void StateMenu::Update(){
    EventManager::GetInstance().Update();
}