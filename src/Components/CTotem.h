#pragma once

#include "../../lib/glm/vec3.hpp"

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
    //CShield(glm::vec3 _pos, int type,int id);
    ~CTotem();

    bool active = false;
    time_point<system_clock> timeStart;        // reloj que contaviliza, cada vez que tenemos el totem, el tiempo
    const int64_t durationTime = 30000;       // 30 seg, debes mantenerlo en tu poder
    int64_t accumulatedTime = 0;
    
};