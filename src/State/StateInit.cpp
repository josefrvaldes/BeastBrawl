#include "StateInit.h"

#include <iostream>

using namespace std;

StateInit::StateInit(){

    std::cout << "> INIT constructor" << std::endl;

    renderEngine = RenderFacadeManager::GetInstance()->GetRenderFacade();
    inputEngine = InputFacadeManager::GetInstance()->GetInputFacade();
    renderEngine->FacadeInitIntro();
    InitState();

}

void StateInit::InitState() {

    if (!soundEngine){
        soundEngine = SoundFacadeManager::GetInstance()->GetSoundFacade();
    }
    soundEngine->SetState(0);

}


void StateInit::Render(){
    renderEngine->FacadeBeginScene();
    renderEngine->FacadeDrawIntro();
    renderEngine->FacadeEndScene();
}


void StateInit::Input(){
    inputEngine->CheckInputIntro();
    renderEngine->FacadeCheckInputIntro();
}


void StateInit::Update(){
    EventManager::GetInstance().Update();
}