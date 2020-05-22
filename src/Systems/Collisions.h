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

#include <Components/CType.h>
#include <Components/CId.h>
#include <Components/CTransformable.h>
#include <Components/CCamera.h>
#include <Components/Component.h>
#include <Components/CCar.h>
#include <Components/CDimensions.h>
#include <Components/CPowerUp.h>

#include <Managers/ManCar.h>
#include <Managers/ManPowerUp.h>
#include <Managers/ManBoxPowerUp.h>
#include <Managers/ManTotem.h>
#include <Managers/ManNavMesh.h>

#include <Entities/Entity.h>
#include <math.h>
#include <memory>
#include <vector>


using namespace std;
#define PI 3.141592

class Collisions
{
public:
    Collisions();
    ~Collisions(){};
    bool Intersects(Entity* ent1,Entity* ent2);
    //void IntersectsCarsPowerUps(ManCar* manCars, ManPowerUp* manPowerUps, ManNavMesh* manNavMesh);
    // void IntersectPlayerPowerUps(Car* carPlayer, ManPowerUp* manPowerUps, ManNavMesh* manNavMesh);
    void IntersectPlayerTotem(Car* carPlayer, ManTotem* manTotem);
    void IntersectCarsTotem(ManCar* manCars, ManTotem* manTotem);
    void IntersectPlayerBoxPowerUp(Car* carPlayer, ManBoxPowerUp* manBoxPowerUp);
    void IntersectCarsBoxPowerUp(ManCar* manCars, ManBoxPowerUp* manBoxPowerUp);

protected:
private:
    
};