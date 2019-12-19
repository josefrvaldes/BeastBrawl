#include "CBoundingSphere.h"

// https://www.youtube.com/watch?v=1l-k8c7NpQM&list=PLEETnX-uPtBXm1KEr_2zQ6K_0hoGH6JJ0&index=3
IntersectData CBoundingSphere::IntersectSphere(const CBoundingSphere &other) const {
    float radiusDistance = radius + other.radius;  // esto es la distancia máxima que tiene que haber entre los dos centros si se están tocando
    vec3 direction = other.center - center; // te da la dirección al otro bounding en x, y, z, es decir, si tenemos 200, 10, 30, significa que estamos a 200 de distancia en x, a 10 en y y a 30 en z
    //vec3 vecCenterDistance = (other.center - center);  // distancia entre los centros
    float centerDistance = glm::length(direction); // longitud del vector
    direction /= centerDistance; // normalizamos la dirección

    float distance = centerDistance - radiusDistance;  // distancia que están intersectando
    bool intersects = distance < 0;
    return IntersectData(intersects, distance * direction);
}