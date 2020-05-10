#include "StateTournamentOptions.h"

#include <iostream>

StateTournamentOptions::StateTournamentOptions(){

    std::cout << "> TOURNAMENT OPTIONS constructor" << std::endl;

    renderEngine = RenderFacadeManager::GetInstance()->GetRenderFacade();
    renderEngine->FacadeInitTournamentOptions();

    InitState();

}

void StateTournamentOptions::InitState() {

    if (!soundEngine){
        soundEngine = SoundFacadeManager::GetInstance()->GetSoundFacade();
    }
    soundEngine->SetState(3);
}


void StateTournamentOptions::Render(){
    renderEngine->FacadeBeginScene();
    renderEngine->FacadeDrawTournamentOptions();
    renderEngine->FacadeEndScene();
}


void StateTournamentOptions::Input(){
    renderEngine->FacadeCheckInputTournamentOptions();
}


void StateTournamentOptions::Update(){
    EventManager::GetInstance().Update();
}