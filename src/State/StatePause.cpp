#include "StatePause.h"
#include <iostream>

using namespace std;

StatePause::StatePause(){
    // constructor
    std::cout << "Estado Pause Creado" << std::endl;
    // Inicializamos las facadas
    renderFacadeManager = RenderFacadeManager::GetInstance();
    //renderFacadeManager->InitializeIrrlicht();
    renderEngine = renderFacadeManager->GetRenderFacade();

    renderEngine->FacadeInitPause();

}

void StatePause::InitState() {

}


void StatePause::Render(){
    renderEngine->FacadeDrawPause();
}


void StatePause::Input(){
    renderEngine->FacadeCheckInputPause();
}


void StatePause::Update(){
}