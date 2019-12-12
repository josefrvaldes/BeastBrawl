#include "CNitro.h"

CNitro::CNitro(){
    m_compType = CompType::NitroComp;
    activePowerUp = false;
}

CNitro::~CNitro(){
    
}


void CNitro::activatePowerUp(){
    activePowerUp = true;
    nitroAcceleration = 0.20; 
    nitroMaxSpeed = 10;
}


void CNitro::deactivePowerUp(){
    activePowerUp = false;
    nitroAcceleration = 0;
    nitroMaxSpeed = 0;
}