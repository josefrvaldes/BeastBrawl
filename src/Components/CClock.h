#pragma once

#include "Component.h"

#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;
//using namespace glm;

class CClock : public Component{
public:
    CClock();
    ~CClock(){};

    bool active;
    time_point<system_clock> timeStart;        // reloj que contaviliza, cada vez que tenemos el totem, el tiempo
};