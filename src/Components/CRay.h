#pragma once

#include "Component.h"

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include <CLPhysics/IntersectData.h>

using namespace glm;

class CBoundingSphere;

class CRay : public Component {
   public:
    CRay() {
        m_compType = CompType::CompRay;
    }

    CRay(const vec3 &_center, const vec3 &direction_)
        : center{_center}, direction{direction_} {
        m_compType = CompType::CompRay;
    }

    float IntersectSphere(const CBoundingSphere &other) const;
    
    vec3 center;
    vec3 direction;
};