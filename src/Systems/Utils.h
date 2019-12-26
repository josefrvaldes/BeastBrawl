#pragma once

#include "lib/glm/vec3.hpp"
#include <math.h>

using namespace glm;
using namespace std;

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

    static float AngleBetweenTwoAngles(float alpha, float beta) {
        float phi = static_cast<int>(fabs(beta - alpha)) % 360;  // This is either the distance or 360 - distance
        int distance = phi > 180 ? 360 - phi : phi;
        return distance;
    }
};