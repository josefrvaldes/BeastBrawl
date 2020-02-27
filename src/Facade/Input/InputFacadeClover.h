#pragma once

#include <iostream>
#include <memory>
#include "InputFacade.h"

#include "../../../CLEngine/src/CLEngine.h"

using namespace std;
using namespace CLE;

class InputFacadeClover : public InputFacade{
    public:
        InputFacadeClover();
        ~InputFacadeClover();
        void CheckInput()        override;
        void CheckInputMenu()    override;
        void CheckInputPause()   override;
        void CheckInputEndRace() override;
        void SetWindowContext(CLEngine* d) {device = d;}

    private:
      CLEngine* device;


};