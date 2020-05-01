#include "CHurt.h"

#include "CCar.h"
#include "CTransformable.h"



CHurt::CHurt() {
    m_compType = CompType::HurtComp;
}

void CHurt::Hurt(CCar *cCar, CTransformable *cTransformable) {
    // si la rotación de ruedas es negativa, significa que estábamos girando a izquierda
    if (cCar->wheelRotation < 0) {
        SetLeft(true);
    } else {
        SetLeft(false);
    }
    hurt = true;
    originalCarRotation = cTransformable->rotation.y;
}

void CHurt::SetLeft(bool left_) {
    left = left_;
    if(left_) {
        leftMultiplier = -1;
    } else {
        leftMultiplier = 1;
    }
}