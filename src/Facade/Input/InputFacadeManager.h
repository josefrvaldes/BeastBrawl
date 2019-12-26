#pragma once

#include <iostream>
#include <memory>
#include "InputFacade.h"
#include "InputFacadeIrrlicht.h"

using namespace std;
class InputFacadeManager{

public:
    ~InputFacadeManager(){};

    void InitializeIrrlicht() {
        facade = new InputFacadeIrrlicht();
    }

    void InitializeClover() {
        // TODO: crear motor de clover
    }

    InputFacade* GetInputFacade() { return facade; };
    static InputFacadeManager* GetInstance();

private:
    InputFacadeManager() : facade(nullptr){};
    InputFacade* facade;
    static InputFacadeManager* instance;
};
