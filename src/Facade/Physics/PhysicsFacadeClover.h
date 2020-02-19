#pragma once

#include "PhysicsFacade.h"

#include <iostream>
#include <memory>

using namespace std;
class PhysicsFacadeClover : public PhysicsFacade {
   public:
    PhysicsFacadeClover();
    ~PhysicsFacadeClover();
    void UpdateCar(Entity*, Entity*) override;
    void UpdateCarAI(Entity*) override;
    void UpdatePowerUps(Entity*) override;

   private:
    void UpdateCam(Entity*);
};