#include "CLastPosition.h"

CLastPosition::CLastPosition(){
    m_compType = CompType::LastPositionComp;
    position.x = 0;
    position.y = 0;
    position.z = 0;
}

CLastPosition::CLastPosition(glm::vec3 _pos)
: position(_pos){
    m_compType = CompType::LastPositionComp;
}

CLastPosition::~CLastPosition(){
    
}