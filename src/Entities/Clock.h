#pragma once

#include "Entity.h"
#include <iostream>

class Clock : public Entity {
   public:
    Clock(uint32_t durationTime_);
    ~Clock() = default; 
};
