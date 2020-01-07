#pragma once

#include <iostream>
#include <map>
#include "../../lib/glm/vec3.hpp"
#include "Entity.h"

using namespace std;
using namespace glm;

class BoundingWall : public Entity {
   public:
    BoundingWall();
    BoundingWall(const vec3 &a_, const vec3 &b_, const vec3 &c_, const vec3 &d_);
    BoundingWall(const vec3 &pos, bool orientation1, bool orientation2);
    BoundingWall(const vec3 &pos);
    ~BoundingWall();
};
