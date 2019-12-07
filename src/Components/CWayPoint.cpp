#include "CWayPoint.h"

CWayPoint::CWayPoint(){
    m_compType = CompType::WayPointComp;
    position.x = 0;
    position.y = 0;
    position.z = 0;
}

CWayPoint::CWayPoint(glm::vec3 _pos)
: position(_pos){
    m_compType = CompType::WayPointComp;
}

CWayPoint::~CWayPoint(){
    
}