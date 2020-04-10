#pragma once

#include "State.h"

#include <EventManager/EventManager.h>
#include <Facade/Render/RenderFacadeManager.h>



class StateGameOptions : public State {
    public:
        StateGameOptions();
        ~StateGameOptions() override = default;
        void Input() override;
        void InitState() override;
        void Update() override;
        void Render() override;
        States GetState() override { return State::States::GAME_OPTIONS; };

    private:
        RenderFacade* renderEngine = { nullptr };
};

