#pragma once

#include "Manager.h"

#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include <Aliases.h>
#include <glm/vec3.hpp>

class PowerUp;
class Data;
class SystemOnline;
class ManCar;

using namespace std;

class ManPowerUp : public Manager{  
   public:
    ManPowerUp(shared_ptr<ManCar> manCars_);
    ~ManPowerUp();
    void SetSystemOnline(SystemOnline *sys) {
        systemOnline = sys;
    };
    
    //vector<shared_ptr<PowerUp>> GetEntities() const {
    //    return PowerUps;
    //};
    void Update();

   private:
	//vector<shared_ptr<PowerUp>> PowerUps;
    void SubscribeToEvents();
    void CreatePowerUp(DataMap* d);
    void NewPowerUpReceivedFromServer(DataMap* d);
    void MaterializePowerUp(shared_ptr<PowerUp> powerUp);
    void DeletePowerUp(DataMap* d);

    SystemOnline *systemOnline {nullptr};
    shared_ptr<ManCar> manCars;
};