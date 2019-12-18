#include "StateEndRace.h"
#include <iostream>

using namespace std;

StateEndRace::StateEndRace(){
    // constructor
    std::cout << "Estado EndRace Creado" << std::endl;
    // Inicializamos las facadas
    renderFacadeManager = RenderFacadeManager::GetInstance();
    //renderFacadeManager->InitializeIrrlicht();
    renderEngine = renderFacadeManager->GetRenderFacade();

    renderEngine->FacadeInitEndRace();

}

void StateEndRace::InitState() {

}


void StateEndRace::Render(){
    renderEngine->FacadeDrawEndRace();
}


void StateEndRace::Input(){
    renderEngine->FacadeCheckInputEndRace();
}


void StateEndRace::Update(){
}