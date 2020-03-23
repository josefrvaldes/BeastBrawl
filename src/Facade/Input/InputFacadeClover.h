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
        ~InputFacadeClover() = default;
        vector<Constants::InputTypes> CheckInput()        override;
        void CheckInputMenu(int&, int)    override;
        void CheckInputPause(int&, int)   override;
        void CheckInputEndRace() override;
        void SetWindowContext(CLEngine* d) {device = d;}

    private:
        CLEngine* device;

        bool scapePress { false };
        bool spacePress { false };
        bool upPress { false };
        bool downPress { false };


};