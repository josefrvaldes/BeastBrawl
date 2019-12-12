#pragma once

#include "../../lib/glm/vec3.hpp"

#include "Component.h"
#include <string>
#include <iostream>
#include <chrono>

using namespace std;
using namespace chrono;
//using namespace glm;

class CNitro : public Component{
public:
    CNitro();
    //CNitro(glm::vec3 _pos, int type,int id);
    ~CNitro();

    time_point<system_clock> timeStart;
    const int64_t durationTime = 5000;       // 5 segundos

    float nitroAcceleration = 0.30;
    float nitroMaxSpeed = 10;
    
};