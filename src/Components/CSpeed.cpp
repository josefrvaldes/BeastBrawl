#include "CSpeed.h"

CSpeed::CSpeed() : speed{vec3(0.f, 0.f, 0.f)} {
    m_compType = CompType::SpeedComp;
    // speed = vec3(0.f, 0.f, 0.f);
}

CSpeed::CSpeed(vec3 _speed) : speed{_speed} {
    m_compType = CompType::SpeedComp;
}
