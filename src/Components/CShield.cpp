#include "CShield.h"



CShield::CShield(){
    m_compType = CompType::ShieldComp;
    activePowerUp = false;
}

CShield::~CShield(){
    
}


void CShield::activatePowerUp(){
    activePowerUp = true;
    timeStart = system_clock::now();
}



void CShield::deactivePowerUp(){
    activePowerUp = false;
}