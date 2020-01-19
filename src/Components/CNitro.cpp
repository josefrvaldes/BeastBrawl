#include "CNitro.h"

CNitro::CNitro(){
    m_compType = CompType::NitroComp;
    activePowerUp = false;
}

CNitro::~CNitro(){
    
}


void CNitro::activatePowerUp(){
    activePowerUp = true;
    timeStart = system_clock::now();
    nitroAcceleration = 5.0; 
    nitroMaxSpeed = 500.0; 
}


void CNitro::deactivePowerUp(){
    activePowerUp = false;
    nitroAcceleration = 0;
    nitroMaxSpeed = 0;
}