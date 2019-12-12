#pragma once

#include <iostream>
#include <memory>
#include "PhysicsFacade.h"
#include "PhysicsFacadeIrrlicht.h"

using namespace std;
class PhysicsFacadeManager{

public:
    PhysicsFacadeManager(){};
    ~PhysicsFacadeManager(){};

    void InitializeIrrlicht() {
        facade = make_shared<PhysicsFacadeIrrlicht>();
    }

    void InitializeClover() {
        // TODO: crear motor de clover
    }

    shared_ptr<PhysicsFacade> GetPhysicsFacade() { return facade; };
    static shared_ptr<PhysicsFacadeManager> GetInstance();

private:
    shared_ptr<PhysicsFacade> facade;
    static const shared_ptr<PhysicsFacadeManager> instance;
};
