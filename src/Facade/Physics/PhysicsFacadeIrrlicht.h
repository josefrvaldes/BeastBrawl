#pragma once

#include <iostream>
#include <memory>
#include "../../../lib/irrlicht/irrlicht.h"
#include "../Render/RenderFacadeIrrlicht.h"
#include "../Render/RenderFacadeManager.h"
#include "PhysicsFacade.h"

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