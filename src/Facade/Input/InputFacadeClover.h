#pragma once

#include <iostream>
#include <memory>
#include "InputFacade.h"

using namespace std;

class InputFacadeClover : public InputFacade{
    public:
        InputFacadeClover();
        ~InputFacadeClover();
        void CheckInput()        override;
        void CheckInputMenu()    override;
        void CheckInputPause()   override;
        void CheckInputEndRace() override;

    private:


};