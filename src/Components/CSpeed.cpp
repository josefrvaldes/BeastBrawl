#include "CSpeed.h"

CSpeed::CSpeed()
{
    m_compType = CompType::Speed;
    m_vx = 0;
    m_vy = 0;
    m_vz = 0;
}

CSpeed::CSpeed(float vx, float vy, float vz)
{
    m_compType = CompType::Speed;
    m_vx = vx;
    m_vy = vy;
    m_vz = vz;
}

CSpeed::~CSpeed()
{
    
}