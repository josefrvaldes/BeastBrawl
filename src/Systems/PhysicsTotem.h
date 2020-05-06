#pragma once

#include <Components/CType.h>
#include <Components/CId.h>
#include <Components/CTransformable.h>
#include <Components/CTotem.h>

#include <Entities/Entity.h>


#include <cmath>
using namespace std;

class PhysicsTotem{
    public:
        PhysicsTotem();
        ~PhysicsTotem(){};

        void update(Entity* actualPowerUp);
};