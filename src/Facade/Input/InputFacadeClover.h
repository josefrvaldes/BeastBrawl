#pragma once

#include <iostream>
#include <memory>
#include "InputFacade.h"
#include "../Render/RenderFacadeManager.h"

#include "../../../CLEngine/src/CLEngine.h"

using namespace std;
using namespace CLE;

class InputFacadeClover : public InputFacade{
    public:
        InputFacadeClover();
        ~InputFacadeClover() override = default;
        vector<Constants::InputTypes> CheckInputMulti()        override;
        void CheckInputSingle()  override;
        void CheckInputMenu(int&, int)    override;
        void CheckInputSelectCharacter(int&, int) override;
        void CheckInputPause(int&, int)   override;
        void CheckInputIntro() override;
        void CheckInputEndRace() override;
        void CheckInputLobbyMulti() override;
        void CheckInputController() override;
        void SetWindowContext(CLEngine* d) {device = d;}

    private:
        CLEngine* device;

        enum InputXBox{BUTTON_A, BUTTON_B, BUTTON_X, BUTTON_Y, BUTTON_LB, BUTTON_RB, BUTTON_BACK, BUTTON_START, BUTTON_XBOX, BUTTON_STICK_L, BUTTON_STICK_R, END};
        std::unordered_map<InputXBox, bool> inputsPressed;
        core::array<SJoystickInfo> joystickInfo;
        bool IsInputPressed(InputXBox input);
        void SetValueInput(InputXBox input, bool valuePressed);
};