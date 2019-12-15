#include "CBoundingSphere.h"

// https://www.youtube.com/watch?v=1l-k8c7NpQM&list=PLEETnX-uPtBXm1KEr_2zQ6K_0hoGH6JJ0&index=3
IntersectData CBoundingSphere::IntersectSphere(const CBoundingSphere &other) const {
    float radiusDistance = radius + other.radius;  // esto es la distancia máxima que tiene que haber entre los dos centros si se están tocando
    vec3 vecCenterDistance = (other.center - center);  // distancia entre los centros
    float centerDistance = glm::length(vecCenterDistance); // longitud del vector

    bool intersects = centerDistance < radiusDistance;
    float intersectDistance = centerDistance - radiusDistance;  // distancia que están intersectando
    return IntersectData(intersects, intersectDistance);
}