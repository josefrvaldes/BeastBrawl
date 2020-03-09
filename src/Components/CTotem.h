#pragma once

#include "Component.h"

#include <glm/vec3.hpp>
#include <string>
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;
//using namespace glm;

class CTotem : public Component{
public:
    CTotem();
    ~CTotem();


    bool active = false;
    time_point<system_clock> timeStart;        // reloj que contaviliza, cada vez que tenemos el totem, el tiempo
    
    const uint32_t SEGUNDOS = 1000;
    const uint32_t DURATION_TIME = 30 * SEGUNDOS;       // 30 seg, debes mantenerlo en tu poder
    
    int64_t accumulatedTime = 0;
    uint16_t positionRanking = 0;               //Posicion en la que vas de la carrera
    
};