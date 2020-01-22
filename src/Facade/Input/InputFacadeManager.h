#pragma once

#include <iostream>
#include <memory>
#include "InputFacade.h"
#include "InputFacadeIrrlicht.h"
#include "InputFacadeClover.h"

using namespace std;
class InputFacadeManager{

public:
    ~InputFacadeManager(){};

    void InitializeIrrlicht() {
        facade = new InputFacadeIrrlicht();
    }

    void InitializeClover() {
        facade = new InputFacadeClover();
    }

    InputFacade* GetInputFacade() { return facade; };
    static InputFacadeManager* GetInstance();

private:
    InputFacadeManager() : facade(nullptr){};
    InputFacade* facade;
    static InputFacadeManager* instance;
};
