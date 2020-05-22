/**
 * Beast Brawl
 * Game created as a final project of the Multimedia Engineering Degree in the University of Alicante.
 * Made by Clover Games Studio, with members 
 * Carlos de la Fuente Torres delafuentetorresc@gmail.com,
 * Antonio Jose Martinez Garcia https://www.linkedin.com/in/antonio-jose-martinez-garcia/,
 * Jesús Mas Carretero jmasc03@gmail.com, 
 * Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/, 
 * Rubén Rubio Martínez https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/, 
 * and Jose Valdés Sirvent https://www.linkedin.com/in/jose-f-valdés-sirvent-6058b5a5/ github -> josefrvaldes
 * 
 * 
 * @author Clover Games Studio
 * 
 */
 
 
 
#include "SystemVisionAI.h"

#include "../Components/CBrainAI.h"
#include "../Components/CBoundingOBB.h"
#include "../Components/CTransformable.h"
#include "../Components/CCar.h"
#include "../Components/CBrainAI.h"
#include "../Components/CTotem.h"
#include "../Components/CHurt.h"

#include "../Entities/Entity.h"
#include "../Entities/CarAI.h"
#include "../Entities/Car.h"
#include "../Entities/CarHuman.h"

#include "../Managers/ManCar.h"
#include "../Managers/ManBoxPowerUp.h"
#include "../Managers/ManTotem.h"
#include "../Managers/ManBoundingOBB.h"
#include "../Managers/ManBoundingGround.h"
#include "../Managers/ManBoundingWall.h"


SystemVisionAI::SystemVisionAI(CLPhysics *clPhysics_) : systemVision{make_unique<SystemVision>(clPhysics_)} {
    // systemVision = make_unique<SystemVision>(clPhysics);
}

void SystemVisionAI::AddManager(Manager &m) {
    managers.push_back(&m);
}


void SystemVisionAI::update(CarAI* actualCar){
    auto cBrainAI = static_cast<CBrainAI*>(actualCar->GetComponent(CompType::BrainAIComp).get());
    auto cHurt = static_cast<CHurt*>(actualCar->GetComponent(CompType::HurtComp).get());

    cBrainAI->CleanVisionRange();
    
    if(cHurt->hurt)  // si está herido no calcula lo que tiene en su rango de vision
        return;

    systemVision->SaveCarInVision(actualCar, cBrainAI, static_cast<ManCar*>(managers[0]), static_cast<ManBoundingWall*>(managers[6]), 
                                                static_cast<ManBoundingOBB*>(managers[7]), static_cast<ManBoundingGround*>(managers[8]));
    systemVision->SaveBoxPowerUpInVision(actualCar, cBrainAI, static_cast<ManBoxPowerUp*>(managers[2]), static_cast<ManBoundingWall*>(managers[6]), 
                                                static_cast<ManBoundingOBB*>(managers[7]), static_cast<ManBoundingGround*>(managers[8]));
    systemVision->SaveTotemInVision(actualCar, cBrainAI, static_cast<ManTotem*>(managers[3]), static_cast<ManBoundingWall*>(managers[6]), 
                                                static_cast<ManBoundingOBB*>(managers[7]), static_cast<ManBoundingGround*>(managers[8]));

    //cout << "NumCars: " << cBrainAI->carInVision.size() << "\n";
    //cout << "NumBox: " << cBrainAI->boxInVision.size() << "\n";
    //if(cBrainAI->totemInVision!=nullptr)
    //cout << "Totem: 1\n";
}