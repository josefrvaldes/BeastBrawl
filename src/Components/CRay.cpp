#include "CRay.h"
#include "CBoundingSphere.h"



/**
 * Devuelve:
 *   < 0 si no intersecta
 *   = 0 si es tangente
 *   > 0 si intersecta
 * http://viclw17.github.io/2018/07/16/raytracing-ray-sphere-intersection/
 */
float CRay::IntersectSphere(const CBoundingSphere& other) const {
    vec3 oc = center - other.center;
    float a = dot(direction, direction);
    float b = 2.0 * dot(oc, direction);
    float c = dot(oc, oc) - other.radius * other.radius;
    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-b - sqrt(discriminant)) / (2.0 * a);
    }
}