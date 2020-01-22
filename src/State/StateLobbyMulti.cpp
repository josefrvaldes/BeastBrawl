#include "StateLobbyMulti.h"
#include <iostream>

#include "../Constants.h"
#include "../Online/TCPClient.h"

using namespace std;

StateLobbyMulti::StateLobbyMulti() /*: tcpClient{make_shared<TCPClient>(SERVER_HOST, SERVER_PORT)}*/{

    std::cout << "> LobbyMenu constructor" << std::endl;

    renderEngine = RenderFacadeManager::GetInstance()->GetRenderFacade();
    renderEngine->FacadeInitLobbyMulti();

}



// Cargamos los bancos de sonido Menu.
void StateLobbyMulti::InitState() {
    
   // cout << "~~~ ENTRO A MENU" << endl;
   // if (!soundEngine){
   //     soundEngine = SoundFacadeManager::GetInstance()->GetSoundFacade();
   //     cout << "~~~ SoundEngine en MENU es -> " << soundEngine << endl;
   // }
   // soundEngine->SetState(2);
   // soundEngine->PlayEvent("Musica/menu");

    //cout << "> MENU iniciado" << endl;
}


void StateLobbyMulti::Render(){
    renderEngine->FacadeDrawLobbyMulti();
}


void StateLobbyMulti::Input(){
    renderEngine->FacadeCheckInputLobbyMulti();
}


void StateLobbyMulti::Update(){
    EventManager::GetInstance().Update();

    SendData();
}



void StateLobbyMulti::SendData(){
    //tcpClient->SendDateTime();
}
