#pragma once

#include <iostream>
#include <map>
#include <vector>
#include "../../include/glm/vec3.hpp"
#include "Entity.h"

using namespace std;
using namespace glm;

class BoundingOBB : public Entity {
   public:
    BoundingOBB(const vector<vec3> &, const vec3 &);
    ~BoundingOBB();
};
