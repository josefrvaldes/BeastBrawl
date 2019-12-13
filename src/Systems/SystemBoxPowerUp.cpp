#include "SystemBoxPowerUp.h"

#include "../Components/CBoxPowerUp.h"

    
SystemBoxPowerUp::SystemBoxPowerUp(){

}
SystemBoxPowerUp::~SystemBoxPowerUp(){

}


void SystemBoxPowerUp::update(ManBoxPowerUp* manBoxPowerUp){

    time_point<system_clock> now = system_clock::now();

    for(std::shared_ptr<BoxPowerUp> actualBox : manBoxPowerUp->GetEntities() ){
        auto cBoxPowerUp = static_cast<CBoxPowerUp*>(actualBox.get()->GetComponent(CompType::BoxPowerUpComp).get());
        if(cBoxPowerUp->active == false){
            if(duration_cast<milliseconds>(now - cBoxPowerUp->timeStart).count() > cBoxPowerUp->respawnBox ){
                cBoxPowerUp->active = true;
                manBoxPowerUp->resetBox(actualBox.get());
            }
        }
    }

}
