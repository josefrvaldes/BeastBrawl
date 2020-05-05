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
    time_point<system_clock> timeStart;        // reloj que contabiliza, cada vez que tenemos el totem, el tiempo
    
    const int32_t SEGUNDOS;
    const int32_t DURATION_TIME;       // X seg, debes mantenerlo en tu poder
    
    int64_t accumulatedTime = 0;
    uint16_t positionRanking = 0;               //Posicion en la que vas de la carrera
    
    // son las fisicas de la entidad TOTEM (cuando el totem esta en el suelo)
    float MaxSpeed {350.0};
    float speed {350.0}; // lo inicializamos al valor del MaxSpeed para que cuando se cree no se mueva
    float aceleration {20.0};

};