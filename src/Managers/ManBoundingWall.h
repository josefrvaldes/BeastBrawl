#pragma once

#include "Manager.h"

using namespace std;

class ManBoundingWall : public Manager {
   public:
    ManBoundingWall();
    ~ManBoundingWall();
    void Integrate(float) override;
    void SubscribeToEvents() override;

   private:
};
