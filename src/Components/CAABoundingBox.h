#pragma once

#include "Component.h"

#include <glm/common.hpp>
#include <glm/vec3.hpp>

#include <CLPhysics/IntersectData.h>

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