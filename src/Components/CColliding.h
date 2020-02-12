#pragma once

#include "Component.h"

#include <chrono>
#include <iostream>
#include <string>

typedef std::chrono::high_resolution_clock Clock;

using namespace std;
using namespace chrono;

class CColliding : public Component {
   public:
    CColliding();
    CColliding(bool colliding_);

    bool colliding{false};
    time_point<system_clock> lastTimeCollided{system_clock::now()};
    //int64_t milis = duration_cast<milliseconds>(now - then).count();
};