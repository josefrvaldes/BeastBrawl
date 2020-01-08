#include "CBoundingPlane.h"
#include <math.h>
#include <iostream>
#include "CBoundingSphere.h"

using namespace glm;
using namespace std;

// https://www.youtube.com/watch?v=5BJs02PaSok&list=PLEETnX-uPtBXm1KEr_2zQ6K_0hoGH6JJ0&index=5
// CBoundingPlane CBoundingPlane::Normalize() const {
//     float magnitude = glm::length(normal);
//     return CBoundingPlane(normal / magnitude, distance / magnitude);
// }

CBoundingPlane::CBoundingPlane(const vec3 &a_, const vec3 &b_, const vec3 &c_, const vec3 &d_)
    : a{a_},
      b{b_},
      c{c_},
      d{d_},
      normal{(cross(b - a, c - a))},             // calculamos su normal sin normalizar // para el cálculo de la normal: https://stackoverflow.com/a/1966605/4657738
      normalizedNormal{normalize(normal)},       // calculamos su normal normalizada
      distance{(dot(normalizedNormal, a_))}  // calculamos la distancia del plano a su normal normalizada, la necesitamos
{
    m_compType = CompType::CompBoundingPlane;
}

IntersectData CBoundingPlane::IntersectSphere(const CBoundingSphere &other) const {
    float distanceFromSpCenter = fabs(dot(normalizedNormal, other.center) - distance);
    // cout << "Distance from sphere center " << distanceFromSpCenter << endl;
    float distanceFromSphere = distanceFromSpCenter - other.radius;
    bool intersectsInfinitePlane = distanceFromSphere < 0;
    return IntersectData(intersectsInfinitePlane, normalizedNormal * distanceFromSphere);
}