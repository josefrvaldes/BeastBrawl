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
    CBoundingCilindre(const glm::vec3 &extreme1_, const glm::vec3 &extreme2_, float _radius);

    IntersectData IntersectSphere(const CBoundingSphere &other) const;

    IntersectData IntersectRay(const CTransformable &transCarRay, const glm::vec3 &normalRay) const;

    glm::vec3 extreme1;
    glm::vec3 extreme2;
    glm::vec3 centerMass;
    const float radius{DEFAULT_SPHERE_RADIUS};
    static constexpr float DEFAULT_SPHERE_RADIUS{10.f};


   private:
    glm::vec3 CalculateProyectPointOnRecta(const glm::vec3 &point_) const;
    void CalculateCenterMass(const glm::vec3 &ext1, const glm::vec3&ext2);
    double EuclideanDis(const glm::vec3 &p1, const glm::vec3 &p2) const;

};