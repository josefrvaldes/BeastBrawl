#pragma once

#include <iostream>
#include <map>
#include <memory>
#include "Manager.h"
#include "../../lib/glm/vec3.hpp"

using namespace std;

class ManPowerUp : public Manager {
   public:
    ManPowerUp();
    ~ManPowerUp();

    void CreatePowerUp(glm::vec3 _position);
    void CreatePowerUp();

   private:
    void SubscribeToEvents();
};