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
    void IntersectsCarsPowerUps(ManCar* manCars, ManPowerUp* manPowerUps, ManNavMesh* manNavMesh);
    void IntersectPlayerPowerUps(Car* carPlayer, ManPowerUp* manPowerUps, ManNavMesh* manNavMesh);
    void IntersectPlayerTotem(Car* carPlayer, ManTotem* manTotem);
    void IntersectCarsTotem(ManCar* manCars, ManTotem* manTotem);
    void IntersectPlayerBoxPowerUp(Car* carPlayer, ManBoxPowerUp* manBoxPowerUp);
    void IntersectCarsBoxPowerUp(ManCar* manCars, ManBoxPowerUp* manBoxPowerUp);

protected:
private:
    
};