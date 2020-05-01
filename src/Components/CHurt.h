#pragma once

#include "Component.h"

class CCar;
class CTransformable;

class CHurt : public Component {
   public:
    CHurt();
    ~CHurt(){};

    float currentRotation{0};  // indica la rotación actual que se le sumará al coche
    float originalCarRotation{0};
    const float speedRotationHigh{18};
    const float speedRotationLow{5};
    bool hurt{false};
    const float FINAL_ROTATION{720};

    void Hurt(CCar *, CTransformable *cTrans);
    void SetLeft(bool left_);
    bool GetLeft() { return left; };
    int8_t GetLeftMultiplier() { return leftMultiplier; };
    
   private:
    int8_t leftMultiplier = 1;
    bool left{false};

};