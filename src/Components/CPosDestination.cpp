#include "CPosDestination.h"

CPosDestination::CPosDestination(){
    m_compType = CompType::PosDestination;
    position.x = 0;
    position.y = 0;
    position.z = 0;
    radious = 30.0f;
}

CPosDestination::CPosDestination(glm::vec3 _pos, int _type, int _id)
: position(_pos), id(_id){
    m_compType = CompType::PosDestination;
    radious = 10.0f;
}

CPosDestination::CPosDestination(glm::vec3 _pos, int _type, int _id, float _radious)
: position(_pos), id(_id){
    m_compType = CompType::PosDestination;
    radious = _radious;
}

CPosDestination::~CPosDestination(){
    
}