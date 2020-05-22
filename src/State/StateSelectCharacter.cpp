#include "StateSelectCharacter.h"

#include <iostream>

using namespace std;

StateSelectCharacter::StateSelectCharacter(){
    std::cout << "> SELECT CHARECTER constructor" << std::endl;
    renderEngine = RenderFacadeManager::GetInstance()->GetRenderFacade();
    InitState();
    renderEngine->FacadeInitSelectCharacter();
}

void StateSelectCharacter::InitState() {

    if (!soundEngine){
        soundEngine = SoundFacadeManager::GetInstance()->GetSoundFacade();
    }
    soundEngine->SetState(2);
}


void StateSelectCharacter::Render(){
    renderEngine->FacadeBeginScene();
    renderEngine->FacadeDrawSelectCharacter();
    renderEngine->FacadeDrawAll();
    renderEngine->FacadeEndScene();
}


void StateSelectCharacter::Input(){
    renderEngine->FacadeCheckInputSelectCharacter();
}


void StateSelectCharacter::Update(){
    EventManager::GetInstance().Update();
}

StateSelectCharacter::~StateSelectCharacter(){
    std::cout << "> SELECT CHARECTER destructor" << std::endl;
}