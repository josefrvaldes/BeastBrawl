#pragma once

#include "Component.h"

#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;
//using namespace glm;

class CClock : public Component{
public:
    CClock(const uint32_t durationGame);
    ~CClock(){};

    bool active;
    time_point<system_clock> timeStart;        // reloj que contabiliza, cada vez que tenemos el totem, el tiempo
    const uint32_t DURATION_TIME;
    int64_t accumulatedTime;
};