#pragma once

#include "State.h"

#include <EventManager/EventManager.h>
#include <Facade/Render/RenderFacadeManager.h>
#include <Facade/Sound/SoundFacadeManager.h>



class StatePause : public State{
    public:
        StatePause();
        ~StatePause() override = default;
        void Input() override;
        void InitState() override;
        void Update() override;
        void Render() override;
        States GetState() override { return State::States::PAUSE; };


    private:
        RenderFacade* renderEngine = { nullptr };
        SoundFacade* soundEngine = { nullptr };


	
};

