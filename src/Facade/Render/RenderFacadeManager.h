#pragma once

#include <iostream>
#include "RenderFacade.h"
#include "RenderFacadeIrrlicht.h"

class RenderFacadeManager{

public:
    ~RenderFacadeManager(){ delete facade;};

    void InitializeIrrlicht() {
        facade = new RenderFacadeIrrlicht();
    }

    void InitializeClover() {
        // TODO: crear motor de clover
    }

    RenderFacade *GetRenderFacade() { return facade; };
    static RenderFacadeManager* GetInstance();

private:
    RenderFacadeManager() : facade(nullptr){}
    RenderFacade *facade;
    static RenderFacadeManager* instance;
};
