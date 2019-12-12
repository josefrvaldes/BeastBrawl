#pragma once

#include <iostream>
#include <memory>
#include "InputFacade.h"
#include "InputFacadeIrrlicht.h"

using namespace std;
class InputFacadeManager{

public:
    ~InputFacadeManager(){};
    InputFacadeManager(){};

    void InitializeIrrlicht() {
        facade = make_shared<InputFacadeIrrlicht>();
    }

    void InitializeClover() {
        // TODO: crear motor de clover
    }

    shared_ptr<InputFacade> GetInputFacade() { return facade; };
    static shared_ptr<InputFacadeManager> GetInstance();

private:
    shared_ptr<InputFacade> facade;
    static const shared_ptr<InputFacadeManager> instance;
};
