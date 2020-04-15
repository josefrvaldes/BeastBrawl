#pragma once

#include "State.h"

#include <EventManager/EventManager.h>
#include <Facade/Input/InputFacadeManager.h>
#include <Facade/Physics/PhysicsFacadeManager.h>
#include <Facade/Render/RenderFacadeManager.h>
#include <Facade/Sound/SoundFacadeManager.h>



class StateControls : public State{
    public:
        StateControls();
        ~StateControls() override = default;
        void Input() override;
        void InitState() override;
        void Update() override;
        void Render() override;
        States GetState() override { return State::States::CONTROLS; };


    private:
        RenderFacade* renderEngine = { nullptr };
        SoundFacade* soundEngine = { nullptr };


	
};

