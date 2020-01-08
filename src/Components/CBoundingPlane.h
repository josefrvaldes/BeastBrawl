#pragma once

#include "../../lib/glm/geometric.hpp"
#include "../../lib/glm/vec3.hpp"
#include "../CLPhysics/IntersectData.h"
#include "Component.h"

using namespace glm;

class CBoundingSphere;

class CBoundingPlane : public Component {
   public:
    CBoundingPlane(const vec3 &a_, const vec3 &b_, const vec3 &c_, const vec3 &d_);

    // void Normalize();
    IntersectData IntersectSphere(const CBoundingSphere &other) const;

    IntersectData IntersectPlane(const CBoundingPlane &other) const;

    vec3 a;
    vec3 b;
    vec3 c;
    vec3 d;

    const vec3 normal;
    const vec3 normalizedNormal;
    const float distance;
};