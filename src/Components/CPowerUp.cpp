#include "CPowerUp.h"

CPowerUp::CPowerUp(): typePowerUp(typeCPowerUp::None){
    m_compType = CompType::PowerUpComp;
}

CPowerUp::CPowerUp(typeCPowerUp _typePowerUp): typePowerUp(_typePowerUp){
    m_compType = CompType::PowerUpComp;
}
/*
CPowerUp::CPowerUp(glm::vec3 _pos, int type, int id)
: position(_pos), m_type(type), m_id(id){
    m_compType = CompType::WayPointComp;
    radious = 30.0f;
    padre = nullptr;
    isFrontera = false;
    isInterior = false;
}*/

CPowerUp::~CPowerUp(){
}