#pragma once

#include "../../include/glm/vec3.hpp"

#include "Component.h"
#include <string>
#include <iostream>
#include <chrono>

using namespace std;
using namespace chrono;
//using namespace glm;

class CRoboJorobo : public Component{
public:
    CRoboJorobo();
    ~CRoboJorobo();
    void activatePowerUp();
    void deactivePowerUp();

    bool activePowerUp = false;
};