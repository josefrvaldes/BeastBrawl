#pragma once

#include <iostream>
#include "PhysicsFacade.h"
#include "../Render/RenderFacadeManager.h"
#include "../Render/RenderFacadeIrrlicht.h"
#include "../../../lib/irrlicht/irrlicht.h"

class PhysicsFacadeIrrlicht : public PhysicsFacade{
    public:
        PhysicsFacadeIrrlicht();
        void UpdateCar(Entity*, Entity*) override;
        void UpdateCarAI(Entity*) override;
        ~PhysicsFacadeIrrlicht();


    private:
        void UpdateCam(Entity*);
        RenderFacadeIrrlicht* renderEngineIrrlicht;
        scene::ISceneManager* smgr;
};