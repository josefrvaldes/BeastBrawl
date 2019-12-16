#pragma once

#include "../Components/CType.h"
#include "../Components/CId.h"
#include "../Components/CTransformable.h"
#include "../Components/CCamera.h"
#include "../Components/Component.h"
#include "../Components/CCar.h"
#include "../Components/CDimensions.h"
#include "../Components/CPowerUp.h"

#include "../Managers/ManCar.h"
#include "../Managers/ManPowerUp.h"

#include "../Entities/Entity.h"
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
    bool Intersects(Entity* car,Entity* carAI);
    void IntersectsCarsPowerUps(ManCar* manCars, ManPowerUp* manPowerUps);
    void IntersectPlayerPowerUps(ManCar* manCars, ManPowerUp* manPowerUps);
protected:
private:
    
};