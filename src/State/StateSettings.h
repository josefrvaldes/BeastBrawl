#pragma once

#include "State.h"

#include <EventManager/EventManager.h>
#include <Facade/Render/RenderFacadeManager.h>
#include <Facade/Sound/SoundFacadeManager.h>

class StateSettings : public State {
public:
    StateSettings();
    ~StateSettings() override;
    void Input() override;
    void InitState() override;
    void Update() override;
    void Render() override;
    States GetState() override { return State::States::SETTINGS; };

private:
    RenderFacade* renderEngine { nullptr };
    SoundFacade* soundEngine { nullptr };
};

