#pragma once

#include "Component.h"

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include <CLPhysics/IntersectData.h>

using namespace glm;

struct CTransformable;

class CBoundingSphere : public Component {
   public:
    CBoundingSphere(const vec3 &_center);
    CBoundingSphere(const vec3 &_center, float _radius);

    IntersectData IntersectSphere(const CBoundingSphere &other) const;
    IntersectData IntersectRay(const CTransformable &transCarRay, const vec3 &normalRay) const;
    void SetCenter(vec3 _center) {
        center = _center;
    }
    vec3 center;
    const float radius{DEFAULT_SPHERE_RADIUS};
    //vec4 equationSphere;  // ecuacion general de la esfera
    // ejemplo:  centro(1,0,-1) radio=3  -->     (x-1)² + y² + (z+1)² = 3²

    static constexpr float DEFAULT_SPHERE_RADIUS{10.f};

    private:
    //vec4 equationSphereCenterRadius(const vec3 &center, const float &radius) const;
};