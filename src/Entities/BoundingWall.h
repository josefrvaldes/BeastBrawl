#pragma once

#include "Entity.h"

#include <iostream>
#include <map>
#include "../../include/glm/vec3.hpp"

using namespace std;
using namespace glm;

class BoundingWall : public Entity {
   public:
    BoundingWall();
    BoundingWall(const glm::vec3 &a_, const glm::vec3 &b_, const glm::vec3 &c_, const glm::vec3 &d_);
    BoundingWall(const glm::vec3 &pos, bool orientation1, bool orientation2);
    BoundingWall(const glm::vec3 &pos);
    ~BoundingWall();
};
