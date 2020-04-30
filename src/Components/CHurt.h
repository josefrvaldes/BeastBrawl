#pragma once

#include "Component.h"

class CHurt : public Component {
   public:
    CHurt();
    ~CHurt(){};

    bool hurt{false};
    float currentRotation{0}; // indica la rotación actual que se le sumará al coche
    float originalCarRotation{0}; 
    const float speedRotationHigh{14};
    const float speedRotationLow{5};
    const float FINAL_ROTATION{720};
};