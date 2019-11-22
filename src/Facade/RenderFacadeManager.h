#ifndef __RENDERFACADEMANAGER_H__
#define __RENDERFACADEMANAGER_H__

#include <iostream>
#include "RenderFacade.h"
#include "RenderFacadeIrrlicht.h"

class RenderFacadeManager
{
public:
    RenderFacadeManager() : facade(nullptr){}
    ~RenderFacadeManager(){ delete facade;};

    void InitializeIrrlicht() {
        facade = new RenderFacadeIrrlicht();
    }

    void InitializeClover() {
        // TODO: crear motor de clover
    }

    RenderFacade *GetRenderFacade() { return facade; };

private:
    RenderFacade *facade;
};

#endif