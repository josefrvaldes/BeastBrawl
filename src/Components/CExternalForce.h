#pragma once

#include "Component.h"
#include <glm/vec3.hpp>

using namespace glm;

class CExternalForce : public Component {
   public:
    CExternalForce();

    vec3 dirExternalForce;
    float force;
    float friction = 6.0 /* *deltaTime */;  // tiempo que tarda en reducirse el choque
};