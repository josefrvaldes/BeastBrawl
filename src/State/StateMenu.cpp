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

void StateMenu::InitState() {

}


void StateMenu::Render(){
    renderEngine->FacadeDrawMenu();
}


void StateMenu::Input(){
    renderEngine->FacadeCheckInputMenu();
}


void StateMenu::Update(){
}