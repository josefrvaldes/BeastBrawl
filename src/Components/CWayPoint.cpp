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
    if(type == 1) // si el tipo es de PowerUp, nos aseguramos que pasamos por el
        radious = 10.0f;
    else
        radious = 20.0f;
    
}

CWayPoint::~CWayPoint(){
    
}