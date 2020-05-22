/**
 * Beast Brawl
 * Game created as a final project of the Multimedia Engineering Degree in the University of Alicante.
 * Made by Clover Games Studio, with members 
 * Carlos de la Fuente Torres delafuentetorresc@gmail.com,
 * Antonio Jose Martinez Garcia https://www.linkedin.com/in/antonio-jose-martinez-garcia/,
 * Jesús Mas Carretero jmasc03@gmail.com, 
 * Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/, 
 * Rubén Rubio Martínez https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/, 
 * and Jose Valdés Sirvent https://www.linkedin.com/in/jose-f-valdés-sirvent-6058b5a5/ github -> josefrvaldes
 * 
 * 
 * @author Clover Games Studio
 * 
 */
 
 
 #pragma once

#include "Manager.h"

#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include <Aliases.h>
#include <glm/vec3.hpp>

class PowerUp;
class PhysicsPowerUp;
class Data;
class SystemOnline;
class ManCar;
enum class typeCPowerUp;

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
    void SubscribeToEvents();
    void CreatePowerUp(DataMap* d);
    void NewPowerUpReceivedFromServer(DataMap* d);
    void MaterializePowerUp(shared_ptr<PowerUp> powerUp, typeCPowerUp type);

    void DeletePowerUps();
    void UpdatePhysics();

    SystemOnline *systemOnline {nullptr};
    unique_ptr<PhysicsPowerUp> physicsPowerUp;
    shared_ptr<ManCar> manCars;
};