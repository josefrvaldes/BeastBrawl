#pragma once

#include "../Components/CType.h"
#include "../Components/CId.h"
#include "../Components/CTransformable.h"
#include "../Components/CCamera.h"
#include "../Components/Component.h"
#include "../Components/CCar.h"
#include "../Components/CDimensions.h"

#include "../Entities/Entity.h"
#include <math.h>
#include <memory>

using namespace std;
#define PI 3.141592

class Collisions
{
public:
    Collisions();
    ~Collisions(){};
    bool Intersects(Entity* car,Entity* carAI);

protected:
private:
    
};