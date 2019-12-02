#pragma once

#include <iostream>
#include "InputFacade.h"
#include "InputFacadeIrrlicht.h"

class InputFacadeManager{

public:
    ~InputFacadeManager(){ delete facade;};

    void InitializeIrrlicht() {
        facade = new InputFacadeIrrlicht();
    }

    void InitializeClover() {
        // TODO: crear motor de clover
    }

    InputFacade *GetInputFacade() { return facade; };
    static InputFacadeManager* GetInstance();

private:
    InputFacadeManager() : facade(nullptr){}
    InputFacade *facade;
    static InputFacadeManager* instance;
};
