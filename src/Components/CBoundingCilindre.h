/**
 * Beast Brawl
 * Game created as a final project of the Multimedia Engineering Degree in the University of Alicante.
 * Made by Clover Games Studio, with members 
 * Carlos de la Fuente Torres delafuentetorresc@gmail.com,
 * Antonio Jose Martinez Garcia https://www.linkedin.com/in/antonio-jose-martinez-garcia/,
 * Jesús Mas Carretero jmasc03@gmail.com, 
 * Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/, 
 * Rubén Rubio Martínez https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/, 
 * and Jose Valdés Sirvent https://www.linkedin.com/in/jose-f-valdés-sirvent-6058b5a5/ github -> josefrvaldes
 * 
 * 
 * @author Clover Games Studio
 * 
 */
 
 
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