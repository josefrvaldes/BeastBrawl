#include "CType.h"

CType::CType(ModelType model)
: m_type(model)
{
    m_compType = CompType::TypeComp;
}