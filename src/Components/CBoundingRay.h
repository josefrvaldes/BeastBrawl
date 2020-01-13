#pragma once

#include "../../lib/glm/vec2.hpp"
#include "../../lib/glm/vec3.hpp"
#include "Component.h"
#include "../Components/CBoundingPlane.h"
#include <iostream>

class CBoundingRay : public Component {
   public:
    CBoundingRay();
    ~CBoundingRay(){};

    
    glm::vec3 normalizedNormal;
    const float baseDistance = 50.0;                   // lo largo base que es el rayo

    // para actuar con planos
    uint32_t iteratorSamePlane;                 // iterador para cambiar del plano de colision
    const uint32_t maxIteratorSamePlane = 30;   // ciclos que tarda en cambiar de plano
    CBoundingPlane* previousPlane;
    glm::vec2 vectorForce;
    glm::vec3 target;                           // punto al que tiene que dirigirse, resultado de la normal del plano
};