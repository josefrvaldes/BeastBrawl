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
 
 
 
#include "SystemData.h"

#include "../Managers/ManCar.h"
#include "../Managers/ManBoxPowerUp.h"
#include "../Managers/ManTotem.h"
#include "../Managers/ManBoundingWall.h"
#include "../Managers/ManBoundingOBB.h"
#include "../Managers/ManBoundingGround.h"


SystemData::SystemData(CLPhysics * clPhysics) : systemVision{make_unique<SystemVision>(clPhysics)} {
    // systemVision = make_unique<SystemVision>(clPhysics);
}

void SystemData::AddManager(Manager &m) {
    managers.push_back(&m);
}


Car* SystemData::CalculateCenteredCar(Entity* actualCar, int visionRange) const{
    return systemVision->CalculateCentredCar(actualCar, visionRange, static_cast<ManCar*>(managers[0]), static_cast<ManBoundingWall*>(managers[6]), 
                                                static_cast<ManBoundingOBB*>(managers[7]), static_cast<ManBoundingGround*>(managers[8]));
}


/*void SystemData::update(CarAI* actualCar){
    auto cBrainAI = static_cast<CBrainAI*>(actualCar->GetComponent(CompType::BrainAIComp).get());

    cBrainAI->CleanVisionRange();

    systemVision->SaveCarInVision(actualCar, cBrainAI, static_cast<ManCar*>(managers[0]), static_cast<ManBoundingOBB*>(managers[7]));
    systemVision->SaveBoxPowerUpInVision(actualCar, cBrainAI, static_cast<ManBoxPowerUp*>(managers[2]), static_cast<ManBoundingOBB*>(managers[7]));
    systemVision->SaveTotemInVision(actualCar, cBrainAI, static_cast<ManTotem*>(managers[3]), static_cast<ManBoundingOBB*>(managers[7]));

    // cout << "NumCars: " << cBrainAI->carInVision.size() << "\n";
    // cout << "NumBox: " << cBrainAI->boxInVision.size() << "\n";
    //if(cBrainAI->totemInVision!=nullptr)
    //    cout << "Totem: 1\n";
}*/