#include "SystemBoxPowerUp.h"

    
SystemBoxPowerUp::SystemBoxPowerUp(){

}
SystemBoxPowerUp::~SystemBoxPowerUp(){

}


void SystemBoxPowerUp::update(ManBoxPowerUp* manBoxPowerUp){

    time_point<system_clock> now = system_clock::now();

    for(std::shared_ptr<BoxPowerUp> actualBox : manBoxPowerUp->GetEntities() ){
        if(actualBox.get()->active == false){
            if(duration_cast<milliseconds>(now - actualBox.get()->timeStart).count() > actualBox.get()->respawnBox ){
                actualBox.get()->active = true;
                manBoxPowerUp->resetBox(actualBox.get());
            }
        }
    }

}
