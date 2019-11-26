#pragma once

#include <iostream>
#include "InputFacade.h"
#include "InputFacadeIrrlicht.h"

class InputFacadeManager
{
public:
    InputFacadeManager() : facade(nullptr){}
    ~InputFacadeManager(){ delete facade;};

    void InitializeIrrlicht() {
        facade = new InputFacadeIrrlicht();
    }

    void InitializeClover() {
        // TODO: crear motor de clover
    }

    InputFacade *GetInputFacade() { return facade; };

private:
    InputFacade *facade;
};
