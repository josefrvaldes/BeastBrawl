/**
 * Beast Brawl
 * Game created as a final project of the Multimedia Engineering Degree in the University of Alicante.
 * Made by Clover Games Studio, with members 
 * Carlos de la Fuente Torres delafuentetorresc@gmail.com,
 * Antonio Jose Martinez Garcia https://www.linkedin.com/in/antonio-jose-martinez-garcia/,
 * Jesús Mas Carretero jmasc03@gmail.com, 
 * Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/, 
 * Rubén Rubio Martínez https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/, 
 * and Jose Valdés Sirvent https://www.linkedin.com/in/jose-f-valdés-sirvent-6058b5a5/ github -> josefrvaldes
 * 
 * 
 * @author Antonio Jose Martinez Garcia
 * @author Jose Valdés Sirvent
 * 
 * 
 */


#pragma once

#include "../EventManager/EventManager.h"
#include "../Facade/Input/InputFacadeManager.h"
#include "../Facade/Physics/PhysicsFacadeManager.h"
#include "../Facade/Render/RenderFacadeManager.h"
#include "../Facade/Sound/SoundFacadeManager.h"
#include "../Facade/Sound/SoundFacade.h"
//#include "../Game.h"
#include "../State/State.h"
#include "../Constants.h"

struct TCPClient;
enum StatesLobbyMulti { CONNECTING, 
                        SERVER_FULL,
                        SELECTING_CHARACTER,
                        WAIT_CHARACTER_RESPONSE,
                        WAITING_OTHER };

class StateLobbyMulti : public State{
   public:
    StateLobbyMulti();
    ~StateLobbyMulti();
    void Input() override;
    void InitState() override;
    void Update() override;
    void Render() override;
    States GetState() override { return State::States::LOBBY_MULTI; };

   private:
    void SubscribeToEvents();
    void StartGameMulti(DataMap* d);
    void ShowDisconnection(DataMap* d);
    void ShowSelectCharacter(DataMap* d);
    void SendCharacterRequest(DataMap* d);
    void ChangeToWait(DataMap* d);
    void ChangeToSelChar(DataMap* d);
    void ReturnToSelChar(DataMap* d);
    void Timer();
    RenderFacade* renderEngine = { nullptr };
    SoundFacade* soundEngine = { nullptr };

    uint64_t valueTimer = 0;
    uint64_t valueMaxTimer = 60 * 5;  // framerate * 5 seconds

    StatesLobbyMulti actualState {StatesLobbyMulti::CONNECTING};

    shared_ptr<TCPClient> tcpClient;	
};

