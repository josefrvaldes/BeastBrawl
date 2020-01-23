#pragma once

#include "../EventManager/EventManager.h"
#include "../Facade/Input/InputFacadeManager.h"
#include "../Facade/Physics/PhysicsFacadeManager.h"
#include "../Facade/Render/RenderFacadeManager.h"
#include "../Facade/Sound/SoundFacadeManager.h"
#include "../Facade/Sound/SoundFacade.h"
//#include "../Game.h"
#include "../State/State.h"

struct TCPClient;

class StateLobbyMulti : public State{
   public:
    StateLobbyMulti();
    ~StateLobbyMulti(){};
    void Input() override;
    void InitState() override;
    void Update() override;
    void Render() override;
    States GetState() { return State::States::LOBBY_MULTI; };

    void SendData();
   private:
    RenderFacade* renderEngine = { nullptr };
    SoundFacade* soundEngine = { nullptr };

    shared_ptr<TCPClient> tcpClient;	
};

