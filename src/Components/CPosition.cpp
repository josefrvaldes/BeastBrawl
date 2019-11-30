#include "CPosition.h"

CPosition::CPosition()
{
    m_compType = CompType::PositionComp;
    x = 0;
    y = 0;
    z = 0;
}

CPosition::CPosition(float _x, float _y, float _z)
: x(_x), y(_y), z(_z)
{
    m_compType = CompType::PositionComp;
}

CPosition::~CPosition()
{
    
}