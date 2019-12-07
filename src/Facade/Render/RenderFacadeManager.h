#pragma once

#include <iostream>
#include "RenderFacade.h"
#include "RenderFacadeIrrlicht.h"

#include <memory>

using namespace std;

class RenderFacadeManager{

public:
    ~RenderFacadeManager(){};
    RenderFacadeManager(){};

    void InitializeIrrlicht() {
        facade = make_shared<RenderFacadeIrrlicht>();
    }

    void InitializeClover() {
        // TODO: crear motor de clover
    }

    shared_ptr<RenderFacade> GetRenderFacade() { return facade; };
    static shared_ptr<RenderFacadeManager> GetInstance();

private:
    shared_ptr<RenderFacade> facade;
    static const shared_ptr<RenderFacadeManager> instance;
};
