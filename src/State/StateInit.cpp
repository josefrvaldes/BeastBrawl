#include "StateInit.h"

#include <iostream>

using namespace std;

StateInit::StateInit(){

    renderEngine = RenderFacadeManager::GetInstance()->GetRenderFacade();
    inputEngine = InputFacadeManager::GetInstance()->GetInputFacade();
    renderEngine->FacadeInitIntro();
    InitState();

    std::cout << ">>>>> Estado Init Creado" << std::endl;
}

void StateInit::InitState() {

    cout << "~~~ ENTRO A STATE INIT" << endl;

    /*if (!soundEngine){
        soundEngine = SoundFacadeManager::GetInstance()->GetSoundFacade();
    }
    soundEngine->SetState(0);
    soundEngine->PlayEvent("Musica/menu");*/
}


void StateInit::Render(){
    renderEngine->FacadeBeginScene();
    renderEngine->FacadeDrawIntro();
    renderEngine->FacadeEndScene();
}


void StateInit::Input(){
    renderEngine->FacadeCheckInputIntro();
}


void StateInit::Update(){
    EventManager::GetInstance().Update();
}