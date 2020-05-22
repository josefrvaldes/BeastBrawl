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
 
 
 #include "PhysicsTotem.h"
#include "../Constants.h"
#include "Utils.h"


PhysicsTotem::PhysicsTotem(){

}

void PhysicsTotem::update(Entity* totem){
   // lo dejamos para cuando tengamos que animarlo
      auto cTotem = static_cast<CTotem*>(totem->GetComponent(CompType::TotemComp).get());
        auto cTransformable = static_cast<CTransformable*>(totem->GetComponent(CompType::TransformableComp).get());
      if(cTotem->speed < cTotem->MaxSpeed){
        // Movimiento
        float angleRotation = (cTransformable->rotation.y * M_PI) / 180.0;
        cTransformable->position.x -= cos(angleRotation) * 100 * Constants::DELTA_TIME;
        cTransformable->position.z += sin(angleRotation) * 100 * Constants::DELTA_TIME;
        cTotem->speed += cTotem->aceleration;
      }else{
        if(cTotem->active){
          cTransformable->rotation.y += 100 * Constants::DELTA_TIME;
        }
      }
}