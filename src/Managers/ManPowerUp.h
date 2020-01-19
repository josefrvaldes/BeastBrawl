#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include "Manager.h"
#include "../Aliases.h"
#include "Manager.h"
#include "../../include/glm/vec3.hpp"

class PowerUp;
class Data;

using namespace std;

class ManPowerUp : public Manager{  
   public:
    ManPowerUp();
    ~ManPowerUp();

    
    //vector<shared_ptr<PowerUp>> GetEntities() const {
    //    return PowerUps;
    //};

   private:
	//vector<shared_ptr<PowerUp>> PowerUps;
    void SubscribeToEvents();
    void CreatePowerUp(DataMap d);
    void DeletePowerUp(DataMap d);
};