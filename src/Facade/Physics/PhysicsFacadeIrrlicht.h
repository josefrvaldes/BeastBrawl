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
    void Update(Entity*, Entity*) override;
    ~PhysicsFacadeIrrlicht();

   private:
    shared_ptr<RenderFacadeIrrlicht> renderEngineIrrlicht;
    scene::ISceneManager* smgr;
};