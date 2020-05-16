#pragma once

#include "State.h"

#include <EventManager/EventManager.h>
#include <Facade/Input/InputFacadeManager.h>
#include <Facade/Physics/PhysicsFacadeManager.h>
#include <Facade/Render/RenderFacadeManager.h>
#include <Facade/Sound/SoundFacadeManager.h>



class StateSelectCharacter : public State {
    public:
        StateSelectCharacter();
        ~StateSelectCharacter() override;
        void Input() override;
        void InitState() override;
        void Update() override;
        void Render() override;
        States GetState() override { return State::States::SELECT_CHARACTER; };


    private:
        RenderFacade* renderEngine = { nullptr };
        SoundFacade* soundEngine = { nullptr };

};

