#pragma once

#include "../../lib/glm/geometric.hpp"
#include "../../lib/glm/vec3.hpp"
#include "../CLPhysics/IntersectData.h"
#include "Component.h"

using namespace glm;

class CBoundingSphere : public Component {
   public:
    CBoundingSphere(const vec3 &_center, float _radius)
        : center{_center}, radius{_radius} {
        m_compType = CompType::CompBoundingSphere;
    }

    IntersectData IntersectSphere(const CBoundingSphere &other) const;
    void SetCenter(vec3 _center) {
        center = _center;
    }
    vec3 center;
    const float radius;
};