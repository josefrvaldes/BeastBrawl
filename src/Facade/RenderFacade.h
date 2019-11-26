#ifndef __RENDERFACADE_H__
#define __RENDERFACADE_H__

#include <iostream>
#include <cstdint>
#include <string>
#include "../Entities/GameObject.h"
#include "../EventManager/EventManager.h"

class RenderFacade {
    public:
        //RenderFacade();
        virtual ~RenderFacade() = default;
        virtual uint16_t FacadeAddObject(GameObject*) = 0;
        virtual void FacadeAddCamera() = 0;
        virtual bool FacadeRun() = 0;
        virtual uint32_t FacadeGetTime() = 0;
        virtual void FacadeCheckInput(float) = 0;
        virtual int FacadeGetFPS() = 0;
        virtual void FacadeSetWindowCaption(std::string) = 0;
        virtual void FacadeBeginScene() = 0;
        virtual void FacadeDrawAll() = 0;
        virtual void FacadeEndScene() = 0;
        virtual void FacadeDeviceDrop() = 0;
        virtual void FacadeDraw() = 0;

    protected:
        EventManager eventManager;
};


#endif 