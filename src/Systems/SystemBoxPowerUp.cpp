#include "SystemBoxPowerUp.h"

    
SystemBoxPowerUp::SystemBoxPowerUp(){

}
SystemBoxPowerUp::~SystemBoxPowerUp(){

}


void SystemBoxPowerUp::update(ManBoxPowerUp* manBoxPowerUp){

    time_point<system_clock> now = system_clock::now();
    for(auto& actualBox : manBoxPowerUp->GetEntities() ){
        auto cBoxPowerUp = static_cast<CBoxPowerUp*>(actualBox.get()->GetComponent(CompType::BoxPowerUpComp).get());
        // comprobamos si la caja se encuentra cogida, y el tiempo que le falta para el respawn
        if(cBoxPowerUp->active == false){
            if(duration_cast<milliseconds>(now - cBoxPowerUp->timeStart).count() > cBoxPowerUp->respawnBox ){
                cBoxPowerUp->active = true;
                manBoxPowerUp->resetBox(actualBox.get());
            }
        }
    }
}
