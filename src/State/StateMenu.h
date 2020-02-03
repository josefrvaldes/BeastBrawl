#pragma once

#include "../EventManager/EventManager.h"
#include "../Facade/Input/InputFacadeManager.h"
#include "../Facade/Physics/PhysicsFacadeManager.h"
#include "../Facade/Render/RenderFacadeManager.h"
#include "../Facade/Sound/SoundFacadeManager.h"
#include "../Facade/Sound/SoundFacade.h"

#include "../State/State.h"


class StateMenu : public State{
    public:
        StateMenu();
        ~StateMenu() = default;
        void Input() override;
        void InitState() override;
        void Update() override;
        void Render() override;
        States GetState() { return State::States::MENU; };


    private:
        RenderFacade* renderEngine = { nullptr };
        //InputFacade* inputEngine = { nullptr };
        //PhysicsFacade* physicsEngine = { nullptr };
        SoundFacade* soundEngine = { nullptr };


	
};

