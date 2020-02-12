#pragma once

#include "Component.h"

#include <glm/vec3.hpp>
#include <string>
#include <iostream>
#include <chrono>

using namespace std;
using namespace chrono;
//using namespace glm;

class CShield : public Component{
public:
    CShield();
    ~CShield();
    void activatePowerUp();
    void deactivePowerUp();

    time_point<system_clock> timeStart;
    const int64_t durationTime = 10000;       // 10 segundos
    
    bool activePowerUp = false;
};