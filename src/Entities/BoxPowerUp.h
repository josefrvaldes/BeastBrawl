#pragma once

#include "Entity.h"
#include "../../lib/glm/vec3.hpp"
#include <iostream>
#include <map>
#include <chrono>

using namespace std;
using namespace chrono;

class BoxPowerUp : public Entity{

public:

    BoxPowerUp();
    BoxPowerUp(glm::vec3 _position);
    ~BoxPowerUp();

//private:
    bool active = true;
    time_point<system_clock> timeStart;
    const int64_t respawnBox = 10000;       // 10 segundos
};
