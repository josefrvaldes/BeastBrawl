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
