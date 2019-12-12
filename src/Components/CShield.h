#pragma once

#include "../../lib/glm/vec3.hpp"

#include "Component.h"
#include <string>
#include <iostream>
#include <chrono>

using namespace std;
using namespace chrono;
//using namespace glm;

class CShield : public Component{
public:
    CShield();
    //CShield(glm::vec3 _pos, int type,int id);
    ~CShield();

    time_point<system_clock> timeStart;
    const int64_t durationTime = 5000;       // 5 segundos
    
};