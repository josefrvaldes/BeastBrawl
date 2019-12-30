#pragma once

#include "../../lib/glm/geometric.hpp"
#include "../../lib/glm/vec3.hpp"
#include "../CLPhysics/IntersectData.h"
#include "Component.h"

using namespace glm;

class CBoundingSphere;

class CBoundingPlane : public Component {
   public:
    CBoundingPlane(const vec3 &_normal, float _distance) : normal{_normal}, distance{_distance} {
        m_compType = CompType::CompBoundingPlane;
    }

    CBoundingPlane Normalize() const;
    IntersectData IntersectSphere(const CBoundingSphere &other) const;

    IntersectData IntersectPlane(const CBoundingPlane &other) const;

    const vec3 normal;
    const float distance;
};