#pragma once

#include "../CLPhysics/IntersectData.h"
#include "Component.h"
#include "include/glm/common.hpp"
#include "include/glm/vec3.hpp"

using namespace glm;

class CAABoundingBox : public Component {
   public:
    CAABoundingBox(vec3 _minExtents, vec3 _maxExtents) : minExtents{_minExtents}, maxExtents{_maxExtents} {
        m_compType = CompType::CompBoundingAABox;
    }

    IntersectData IntersectCAABoundingBox(CAABoundingBox &other) const;

    vec3 minExtents;  // si fuera un plano 2d, representaría el punto inferior izquierdo
    vec3 maxExtents;  // si fuera un plano 2d, representaría el punto superior derecho
};