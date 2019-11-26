#include "CPosition.h"

CPosition::CPosition()
{
    m_compType = CompType::Position;
    m_x = 0;
    m_y = 0;
    m_z = 0;
}

CPosition::CPosition(float x, float y, float z)
{
    m_compType = CompType::Position;
    m_x = x;
    m_y = y;
    m_z = z;
}

CPosition::~CPosition()
{
    
}