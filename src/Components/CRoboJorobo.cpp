#include "CRoboJorobo.h"

CRoboJorobo::CRoboJorobo(){
    m_compType = CompType::RoboJoroboComp;
    activePowerUp = false;
}

CRoboJorobo::~CRoboJorobo(){
    
}



void CRoboJorobo::activatePowerUp(){
    activePowerUp = true;
}


void CRoboJorobo::deactivePowerUp(){
    activePowerUp = true;
}