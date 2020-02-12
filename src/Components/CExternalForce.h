#pragma once

#include "Component.h"
#include <glm/vec3.hpp>

using namespace glm;

class CExternalForce : public Component {
   public:
    CExternalForce() : force{0}{
        m_compType = CompType::CompExternalForce;
    }

    vec3 dirExternalForce;
    float force;
};