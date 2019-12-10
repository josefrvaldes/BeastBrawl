#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <vector>
//#include "Manager.h"
#include "../../lib/glm/vec3.hpp"

class PowerUp;

using namespace std;

class ManPowerUp{  
   public:
    ManPowerUp();
    ~ManPowerUp();

    void CreatePowerUp(glm::vec3 _position);
    void CreatePowerUp();
    vector<shared_ptr<PowerUp>> GetEntities() const {
        return PowerUps;
    };

   private:
	vector<shared_ptr<PowerUp>> PowerUps;
    void SubscribeToEvents();
};