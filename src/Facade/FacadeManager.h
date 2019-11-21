#ifndef __FACADEMANAGER_H__
#define __FACADEMANAGER_H__

#include <iostream>
#include "Facade.h"
#include "FacadeIrrlicht.h"

class FacadeManager
{
public:
    FacadeManager();
    ~FacadeManager();

    void InitializeIrrlicht() {
        facade = new FacadeIrrlicht();
    }

    void InitializeClover() {
        delete facade;
        // TODO: crear motor de clover
    }

    Facade *GetFacade() { return facade; };

private:
    Facade *facade;
};

#endif