#include "CRemovableObject.h"

CRemovableObject::CRemovableObject() : destroy{false} {
    m_compType = CompType::RemovableObjectComp;
}