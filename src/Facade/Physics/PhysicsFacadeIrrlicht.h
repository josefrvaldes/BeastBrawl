#pragma once

#include "PhysicsFacade.h"

#include <iostream>
#include <memory>
#include <irrlicht/irrlicht.h>
#include <Facade/Render/RenderFacadeIrrlicht.h>
#include <Facade/Render/RenderFacadeManager.h>

using namespace std;
class PhysicsFacadeIrrlicht : public PhysicsFacade {
   public:
    PhysicsFacadeIrrlicht();
    ~PhysicsFacadeIrrlicht();
    void UpdateCar(Entity*, Entity*) override;
    void UpdateCarAI(Entity*) override;
    void UpdatePowerUps(Entity*) override;

   private:
    void UpdateCam(Entity*);
    RenderFacadeIrrlicht* renderEngineIrrlicht;
    scene::ISceneManager* smgr;
};