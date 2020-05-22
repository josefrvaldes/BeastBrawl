#include "CNamePlate.h"

CNamePlate::CNamePlate(int _id, std::string _billboardPath)
: idCarAsociated(_id), billboardPath(_billboardPath)
{
    m_compType = CompType::NamePlateComp;
}