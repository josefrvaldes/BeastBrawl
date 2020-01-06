#pragma once

#include "../../lib/glm/geometric.hpp"
#include "../../lib/glm/vec3.hpp"
#include "../CLPhysics/IntersectData.h"
#include "Component.h"

using namespace glm;

class CBoundingSphere;

class CBoundingPlane : public Component {
   public:
    CBoundingPlane(const vec3 &a_, vec3 &b_, vec3 &c_, vec3 &d_)
        : /*normal{_normal}, distance{_distance}*/
          a{a_},
          b{b_},
          c{c_},
          d{d_},
          normal{normalize(cross(b - a, c - a))} {  // para el cálculo de la normal: https://stackoverflow.com/a/1966605/4657738
        m_compType = CompType::CompBoundingPlane;
    }

    //CBoundingPlane Normalize() const;
    IntersectData IntersectSphere(const CBoundingSphere &other) const;

    IntersectData IntersectPlane(const CBoundingPlane &other) const;

    const vec3 a;
    const vec3 b;
    const vec3 c;
    const vec3 d;

    const vec3 normal;
    //const float distance;
};