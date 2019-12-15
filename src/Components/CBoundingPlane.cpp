#include "CBoundingPlane.h"
#include <math.h>
#include "CBoundingSphere.h"

// https://www.youtube.com/watch?v=5BJs02PaSok&list=PLEETnX-uPtBXm1KEr_2zQ6K_0hoGH6JJ0&index=5
CBoundingPlane CBoundingPlane::Normalize() const {
    float magnitude = glm::length(normal);
    return CBoundingPlane(normal / magnitude, distance / magnitude);
}

IntersectData CBoundingPlane::IntersectSphere(const CBoundingSphere &other) const {
    float distanceFromSpCenter = fabs(glm::dot(normal, other.center) + distance);
    float distanceFromSphere = distanceFromSpCenter - other.radius;
    bool intersects = distanceFromSphere < 0;
    return IntersectData(intersects, distanceFromSphere);
}