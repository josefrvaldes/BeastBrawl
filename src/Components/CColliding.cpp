#include "CColliding.h"

CColliding::CColliding() {
    m_compType = CompType::CollidingComp;
}

CColliding::CColliding(bool colliding_) : colliding{colliding_} {
    m_compType = CompType::CollidingComp;
}