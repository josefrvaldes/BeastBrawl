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

    if (!soundEngine){
        soundEngine = SoundFacadeManager::GetInstance()->GetSoundFacade();
    }
    soundEngine->SetState(1);
}


void StateMenu::Render(){
    renderEngine->FacadeBeginScene();
    renderEngine->FacadeDrawMenu();
    renderEngine->FacadeEndScene();
}


void StateMenu::Input(){
    renderEngine->FacadeCheckInputMenu();
}


void StateMenu::Update(){
    EventManager::GetInstance().Update();
}