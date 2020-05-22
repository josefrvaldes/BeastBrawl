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

#include "Component.h"

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include <CLPhysics/IntersectData.h>

using namespace glm;

struct CTransformable;

class CBoundingSphere : public Component {
   public:
    CBoundingSphere(const vec3 &center_);
    CBoundingSphere(const vec3 &center_, float radius_);
    CBoundingSphere(const vec3 &center_, float radius_, float radiusFloor_);
    CBoundingSphere(const vec3 &center_, float radius_, float radiusFloor_, float distanCenter_);

    IntersectData IntersectSphere(const CBoundingSphere &other) const;
    IntersectData IntersectRay(const CTransformable &transCarRay, const vec3 &normalRay) const;
    void SetCenter(vec3 _center) {
        center = _center;
    }
    vec3 center;
    float disCenter{0.0};
    const float radius{DEFAULT_SPHERE_RADIUS};
    const float radiusFloor{DEFAULT_SPHERE_RADIUS};
    //vec4 equationSphere;  // ecuacion general de la esfera
    // ejemplo:  centro(1,0,-1) radio=3  -->     (x-1)² + y² + (z+1)² = 3²

    static constexpr float DEFAULT_SPHERE_RADIUS{10.f};

    private:
    //vec4 equationSphereCenterRadius(const vec3 &center, const float &radius) const;
};