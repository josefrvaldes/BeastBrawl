#pragma once

#include "Entity.h"
#include "../../lib/glm/vec3.hpp"
#include <iostream>
#include <map>

using namespace std;

class PowerUp : public Entity
{

public:
    PowerUp();
    PowerUp(glm::vec3 _position);
    ~PowerUp();


private:
};
