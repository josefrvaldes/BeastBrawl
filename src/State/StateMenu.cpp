#include "StateMenu.h"
#include <iostream>

using namespace std;

StateMenu::StateMenu(){
    // constructor
    std::cout << "Estado Menu Creado" << std::endl;
    // Inicializamos las facadas
    renderFacadeManager = RenderFacadeManager::GetInstance();
    //renderFacadeManager->InitializeIrrlicht();
    renderEngine = renderFacadeManager->GetRenderFacade();

    renderEngine->FacadeInitMenu();

}

// Cargamos los bancos de sonido Menu.
void StateMenu::InitState() {
    soundEngine = SoundFacadeManager::GetInstance()->GetSoundFacade();
    // TO-DO: Aqui nunca entra en el StateMenu porque soundEngine es 0. El SoundEngine se declara por primera vez en el MainLoop y a este Init se le llama antes.
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