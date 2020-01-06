#pragma once

#include <iostream>
#include <map>
#include "../../lib/glm/vec3.hpp"
#include "Entity.h"

using namespace std;

class BoundingWall : public Entity {
   public:
    BoundingWall();
    ~BoundingWall();
};
