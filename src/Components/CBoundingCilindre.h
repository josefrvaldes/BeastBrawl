#pragma once

#include "../../include/glm/geometric.hpp"
#include "../../include/glm/vec3.hpp"
#include "../CLPhysics/IntersectData.h"
#include "Component.h"

using namespace glm;

struct CTransformable;
struct CBoundingSphere;

class CBoundingCilindre : public Component {
   public:
    CBoundingCilindre(const vec3 &extreme1_, const vec3 &extreme2_, float _radius);

    IntersectData IntersectSphere(const CBoundingSphere &other) const;

    IntersectData IntersectRay(const CTransformable &transCarRay, const vec3 &normalRay) const;

    vec3 extreme1;
    vec3 extreme2;
    const float radius{DEFAULT_SPHERE_RADIUS};
    static constexpr float DEFAULT_SPHERE_RADIUS{10.f};


   private:
    vec3 CalculateProyectPointOnRecta(const vec3 &point_) const;
    double EuclideanDis(const vec3 &p1, const vec3 &p2) const;

};