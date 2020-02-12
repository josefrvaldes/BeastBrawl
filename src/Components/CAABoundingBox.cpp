#include "CAABoundingBox.h"

#include <Systems/Utils.h>

// https://www.youtube.com/watch?v=Iu6nAXFm2Wo&list=PLEETnX-uPtBXm1KEr_2zQ6K_0hoGH6JJ0&index=4
IntersectData CAABoundingBox::IntersectCAABoundingBox(CAABoundingBox &other) const {
    vec3 distances1 = other.minExtents - maxExtents;    // distancia en todos los ejes entre uno y otro
    vec3 distances2 = minExtents - other.maxExtents;    // distancia en todos los ejes entre uno y otro
    vec3 distances = glm::max(distances1, distances2);  // mayor de esas distancias
    float maxDistance = Utils::Vec3MaxValue(distances);
    bool intersects = maxDistance < 0;
    return IntersectData(intersects, distances);
}    