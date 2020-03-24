#include "StateSelectCharacter.h"

#include <iostream>

using namespace std;

StateSelectCharacter::StateSelectCharacter(){

    renderEngine = RenderFacadeManager::GetInstance()->GetRenderFacade();
    renderEngine->FacadeInitSelectCharacter();

    InitState();
    std::cout << ">>>>> Estado SELECT CHARACTER creado" << std::endl;

}

void StateSelectCharacter::InitState() {

    /*cout << "~~~ ENTRO A MENU" << endl;
    if (!soundEngine){
        soundEngine = SoundFacadeManager::GetInstance()->GetSoundFacade();
        cout << "~~~ SoundEngine en MENU es -> " << soundEngine << endl;
    }
    soundEngine->SetState(2);
    soundEngine->PlayEvent("Musica/menu");*/

    //cout << "> MENU iniciado" << endl;
}


void StateSelectCharacter::Render(){
    renderEngine->FacadeBeginScene();
    renderEngine->FacadeDrawSelectCharacter();
    renderEngine->FacadeEndScene();
}


void StateSelectCharacter::Input(){
    renderEngine->FacadeCheckInputSelectCharacter();
}


void StateSelectCharacter::Update(){
    EventManager::GetInstance().Update();
}