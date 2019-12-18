#include "CWayPoint.h"

CWayPoint::CWayPoint(){
    m_compType = CompType::WayPointComp;
    position.x = 0;
    position.y = 0;
    position.z = 0;
    radious = 30.0f;
}

CWayPoint::CWayPoint(glm::vec3 _pos, int _type, int _id)
: position(_pos), type(_type), id(_id){
    m_compType = CompType::WayPointComp;
    radious = 30.0f;
}

CWayPoint::~CWayPoint(){
    
}