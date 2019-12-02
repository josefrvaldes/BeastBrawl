#pragma once

#include <iostream>
#include "PhysicsFacade.h"
#include "PhysicsFacadeIrrlicht.h"

class PhysicsFacadeManager{

public:
    ~PhysicsFacadeManager(){ delete facade;};

    void InitializeIrrlicht() {
        facade = new PhysicsFacadeIrrlicht();
    }

    void InitializeClover() {
        // TODO: crear motor de clover
    }

    PhysicsFacade *GetPhysicsFacade() { return facade; };
    static PhysicsFacadeManager* GetInstance();

private:
    PhysicsFacadeManager() : facade(nullptr){}
    PhysicsFacade *facade;
    static PhysicsFacadeManager* instance;
};
