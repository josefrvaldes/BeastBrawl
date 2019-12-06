#include "CSpeed.h"

CSpeed::CSpeed()
{
    m_compType = CompType::SpeedComp;
    x = 0;
    y = 0;
    z = 0;
}

CSpeed::CSpeed(float vx, float vy, float vz)
{
    m_compType = CompType::SpeedComp;
    x = vx;
    y = vy;
    z = vz;
}

