#pragma once

#include <iostream>
#include "RenderFacade.h"
#include "RenderFacadeIrrlicht.h"
#include "RenderFacadeClover.h"

#include <memory>

using namespace std;

class RenderFacadeManager{

public:
    ~RenderFacadeManager(){delete facade;};

    void InitializeIrrlicht() {
        facade = new RenderFacadeIrrlicht();
    }

    void InitializeClover() {
        facade = new RenderFacadeClover();
    }

    RenderFacade* GetRenderFacade() { return facade; };
    static RenderFacadeManager* GetInstance();

private:
    RenderFacadeManager() : facade(nullptr){};
    RenderFacade* facade;
    static RenderFacadeManager* instance;
};
