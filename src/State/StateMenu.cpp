#include "StateMenu.h"
#include <iostream>

using namespace std;

StateMenu::StateMenu(){
    // constructor
    std::cout << "Estado Menu Creado" << std::endl;
    // Inicializamos las facadas
    renderEngine = RenderFacadeManager::GetInstance()->GetRenderFacade();
    //renderFacadeManager->InitializeIrrlicht();

    renderEngine->FacadeInitMenu();

}

// Cargamos los bancos de sonido Menu.
void StateMenu::InitState() {
    
    cout << "~~~ ENTRO A MENU" << endl;
    soundEngine = SoundFacadeManager::GetInstance()->GetSoundFacade();
    if (soundEngine){
        soundEngine->SetState(2);
    }
}


void StateMenu::Render(){
    renderEngine->FacadeDrawMenu();
}


void StateMenu::Input(){
    renderEngine->FacadeCheckInputMenu();
}


void StateMenu::Update(){
}