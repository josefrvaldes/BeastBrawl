#include "CMovementType.h"

CMovementType::CMovementType(std::string path)
: movementType(path)
{
    m_compType = CompType::MovementComp;
}