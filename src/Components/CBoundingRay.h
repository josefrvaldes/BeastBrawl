#pragma once

#include "../../lib/glm/vec2.hpp"
#include "../../lib/glm/vec3.hpp"
#include "Component.h"
#include "../Components/CBoundingPlane.h"
#include "../Components/CBoundingSphere.h"
#include <iostream>

class CBoundingRay : public Component {
   public:
    CBoundingRay();
    ~CBoundingRay(){};

    const float baseDistance = 50.0;             // lo largo base que es el rayo

    // para evitar atraparse en la esquina de los planos
    uint32_t iteratorSamePlane = 0;                 // iterador para cambiar del plano de colision
    const uint32_t maxItSamePlane = 30;         // ciclos que tarda en cambiar de plano
    CBoundingPlane* previousPlane = nullptr;
    glm::vec3 target = glm::vec3(0,0,0);                           // punto al que tiene que dirigirse, resultado de la normal del plano

    // para evitar vibracion en la evasion de un objeto (coche)
    uint32_t iteratorSameSphere = 0;
    const uint32_t maxItSameSphere = 30;        // ciclos que tarda en cambiar de esfera
    CBoundingSphere* previousSphere = nullptr;
    glm::vec2 vectorForce = glm::vec2(0,0);

};