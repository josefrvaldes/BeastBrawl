#include "CWayPoint.h"

CWayPoint::CWayPoint(){
    m_compType = CompType::WayPointComp;
    position.x = 0;
    position.y = 0;
    position.z = 0;
}

CWayPoint::CWayPoint(glm::vec3 _pos, int type, int id)
: position(_pos), m_type(type), m_id(id){
    m_compType = CompType::WayPointComp;
}

CWayPoint::~CWayPoint(){
    
}