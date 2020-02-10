#pragma once

#include "Component.h"

#include <glm/vec3.hpp>
#include <string>
#include <iostream>
#include <chrono>

using namespace std;
using namespace chrono;
//using namespace glm;

class CBoxPowerUp : public Component{
public:
    CBoxPowerUp();
    //CPowerUp(glm::vec3 _pos, int type,int id);
    ~CBoxPowerUp();

    bool active = true;
    time_point<system_clock> timeStart;
    const int64_t respawnBox = 10000;       // 10 segundos
    
};