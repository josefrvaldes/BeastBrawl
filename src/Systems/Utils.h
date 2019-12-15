#pragma once

#include "lib/glm/vec3.hpp"

using namespace glm;

class Utils {
   public:
    static float Vec3MaxValue(const vec3 &vec) {
        float maxDistance = vec.x;  // max distancia en alguno de los ejes
        if (vec.y > maxDistance)
            maxDistance = vec.y;
        if (vec.z > maxDistance)
            maxDistance = vec.z;
        return maxDistance;
    }
};