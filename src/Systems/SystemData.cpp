
#include "SystemData.h"

#include "../Managers/ManCar.h"
#include "../Managers/ManBoxPowerUp.h"
#include "../Managers/ManTotem.h"
#include "../Managers/ManBoundingWall.h"
#include "../Managers/ManBoundingOBB.h"
#include "../Managers/ManBoundingGround.h"


SystemData::SystemData(){
    systemVision = make_unique<SystemVision>();
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