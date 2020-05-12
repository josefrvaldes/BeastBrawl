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
        // ROTACION
        auto cTransformable = static_cast<CTransformable*>(actualBox.get()->GetComponent(CompType::TransformableComp).get());
        cTransformable->rotation.y += 100 * Constants::DELTA_TIME;

        // ESCALADO
        if (cTransformable->scale.y > 2.0 + offsetScale){
            increase = false;
        }
        if(cTransformable->scale.y < 2.0 - offsetScale){
            increase = true;
        }

        if(increase){
            cTransformable->scale.x += 0.7 * Constants::DELTA_TIME;
            cTransformable->scale.y += 0.7 * Constants::DELTA_TIME;
            cTransformable->scale.z += 0.7 * Constants::DELTA_TIME;
        }    
        else{
            cTransformable->scale.x -= 0.7 * Constants::DELTA_TIME;
            cTransformable->scale.y -= 0.7 * Constants::DELTA_TIME;
            cTransformable->scale.z -= 0.7 * Constants::DELTA_TIME;
        }                    
        


    }
}
