#pragma once

#include "../../include/glm/vec3.hpp"

#include "Component.h"
#include <string>
#include <iostream>
#include <chrono>

using namespace std;
using namespace chrono;
//using namespace glm;

class CTotem : public Component{
public:
    CTotem();
    ~CTotem();

    bool active = false;
    time_point<system_clock> timeStart;        // reloj que contaviliza, cada vez que tenemos el totem, el tiempo
    const int64_t durationTime = 300;       // 30 seg, debes mantenerlo en tu poder
    int64_t accumulatedTime = 0;
    
};