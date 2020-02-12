#pragma once

#include "Component.h"

#include <glm/vec3.hpp>
#include <string>
#include <iostream>
#include <chrono>

using namespace std;
using namespace chrono;
//using namespace glm;

class CNitro : public Component{
public:
    CNitro();
    ~CNitro();
    void activatePowerUp();
    void deactivePowerUp();

    time_point<system_clock> timeStart;
    const int64_t durationTime = 1000;       // 1 segundos

    float nitroAcceleration = 0;            // este valor se le va a sumar al del coche
    float nitroMaxSpeed = 0;                // este valor se le va a sumar al del coche

    bool activePowerUp = false;
    
};