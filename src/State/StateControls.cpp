#include "StateControls.h"
#include <iostream>

using namespace std;

StateControls::StateControls(){

    std::cout << "> CRONTROLS constructor" << std::endl;

    renderEngine = RenderFacadeManager::GetInstance()->GetRenderFacade();
    renderEngine->FacadeInitControler();

}

// Cargamos los bancos de sonido Menu.
void StateControls::InitState() {
    //if (!soundEngine){
    //    soundEngine = SoundFacadeManager::GetInstance()->GetSoundFacade();
    //}
    //soundEngine->SetState(10);
}


void StateControls::Render(){
    renderEngine->FacadeBeginScene();
    renderEngine->FacadeDrawControler();
    renderEngine->FacadeEndScene();
}


void StateControls::Input(){
    renderEngine->FacadeCheckInputControler();
}


void StateControls::Update(){
    EventManager::GetInstance().Update();
}