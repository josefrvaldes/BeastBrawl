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