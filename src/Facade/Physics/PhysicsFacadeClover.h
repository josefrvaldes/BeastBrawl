#pragma once

#include "PhysicsFacade.h"
#include "../Render/RenderFacadeManager.h"

#include "../../../CLEngine/src/CLEngine.h"


#include <iostream>
#include <memory>

using namespace CLE;
using namespace std;
class PhysicsFacadeClover : public PhysicsFacade {
   public:
    PhysicsFacadeClover();
    ~PhysicsFacadeClover();
    void UpdateCar(Entity*, Entity*) override;
    void UpdateTransformable(Entity*) override;
    void UpdatePowerUps(Entity*) override;
    void SetWindowContext(CLEngine* d) { device = d;};

   private:
    void UpdateCam(Entity*);
    CLEngine* device;
    CLNode* smgr;
};