#pragma once

#include "Component.h"

class CHurt : public Component {
   public:
    CHurt();
    ~CHurt(){};

    bool hurt{false};
    int64_t timeStartHurt{0};
    const int64_t MAX_TIME_HURT{1600};
    uint16_t currentRotation{0};
    uint16_t speedRotation{1};
};