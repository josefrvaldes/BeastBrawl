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
 
 
 #include "PhysicsPowerUp.h"
#include "../Constants.h"
#include "Utils.h"


PhysicsPowerUp::PhysicsPowerUp(){

}

    //void init();
void PhysicsPowerUp::update(Entity* actualPowerUp){
   typeCPowerUp actualTypePowerUp = static_cast<CPowerUp*>(actualPowerUp->GetComponent(CompType::PowerUpComp).get())->typePowerUp;
   switch ( actualTypePowerUp ){
      case typeCPowerUp::PudinDeFrambuesa: updatePudinDeFrambuesa(actualPowerUp);
         break;
      case typeCPowerUp::TeleBanana:       updateTeleBanana(actualPowerUp);
         break;
      case typeCPowerUp::MelonMolon:       updateMelonMolon(actualPowerUp);
         break;
      default:
         break;
   }
}


// TO-DO: pequenya animacion del puding para cuando lo lancemos estando quietos, que no nos afecte
void PhysicsPowerUp::updatePudinDeFrambuesa(Entity* pu){
   // lo dejamos para cuando tengamos que animarlo
      auto cPuActual = static_cast<CPowerUp*>(pu->GetComponent(CompType::PowerUpComp).get());
       // Movimiento
      auto cTransformable = static_cast<CTransformable*>(pu->GetComponent(CompType::TransformableComp).get());
      float angleRotation = (cTransformable->rotation.y * PI) / 180.0;
      cTransformable->position.x -= cos(angleRotation) * cPuActual->speed * Constants::DELTA_TIME;
      cTransformable->position.z += sin(angleRotation) * cPuActual->speed * Constants::DELTA_TIME;
      cPuActual->speed -= cPuActual->deceleration;
      if(cPuActual->speed > 0)
         cPuActual->speed = 0; 
}


void PhysicsPowerUp::updateTeleBanana(Entity* pu){
   time_point<system_clock> now = system_clock::now();
   auto cPuActual = static_cast<CPowerUp*>(pu->GetComponent(CompType::PowerUpComp).get());
   auto cTransformable = static_cast<CTransformable*>(pu->GetComponent(CompType::TransformableComp).get());

   if(cPuActual->calculate == false){
      if(duration_cast<milliseconds>(now - cPuActual->timeStart).count() > cPuActual->durationTime ){
            // A partir de aqui el powerUp hace efecto a cualquier Entidad coche del mapa
            cPuActual->calculate = true;
      }
      // Movimiento
      float angleRotation = Utils::GetAdjustedDegrees(cTransformable->rotation.y - cPuActual->rotExtraTelebanana);
      cTransformable->position.x -= cos(glm::radians(angleRotation)) * cPuActual->speed * Constants::DELTA_TIME;
      cTransformable->position.z += sin(glm::radians(angleRotation)) * cPuActual->speed * Constants::DELTA_TIME;

   }else if(static_cast<CTargetEntity*>(pu->GetComponent(CompType::TargetEntityComp).get())->cTransTarget != nullptr){
      // Vector
      auto cTargetCar = static_cast<CTargetEntity*>(pu->GetComponent(CompType::TargetEntityComp).get());
      float vectorX = cTargetCar->cTransTarget->position.x - cTransformable->position.x;
      float vectorZ = cTargetCar->cTransTarget->position.z - cTransformable->position.z;
      // divisor unitario
      float divisorUnitario = sqrt((vectorX*vectorX) + (vectorZ*vectorZ));
      // Movimiento perseguir
      cTransformable->position.x += (vectorX/divisorUnitario) * cPuActual->speed * Constants::DELTA_TIME;
      cTransformable->position.z += (vectorZ/divisorUnitario) * cPuActual->speed * Constants::DELTA_TIME;
   }else{
      // Movimiento como melon molon
      float angleRotation = Utils::GetAdjustedDegrees(cTransformable->rotation.y - cPuActual->rotExtraTelebanana);
      cTransformable->position.x -= cos(glm::radians(angleRotation)) * cPuActual->speed * Constants::DELTA_TIME;
      cTransformable->position.z += sin(glm::radians(angleRotation)) * cPuActual->speed * Constants::DELTA_TIME;
   }
   cPuActual->rotExtraTelebanana = Utils::GetAdjustedDegrees(cPuActual->rotExtraTelebanana + 800 * Constants::DELTA_TIME);
   cTransformable->rotation.y = Utils::GetAdjustedDegrees(cTransformable->rotation.y + 800 * Constants::DELTA_TIME);
}




void PhysicsPowerUp::updateMelonMolon(Entity* pu){
   auto cPuActual = static_cast<CPowerUp*>(pu->GetComponent(CompType::PowerUpComp).get());
   //cPuActual->effectActive = true;
   // Movimiento
   auto cTransformable = static_cast<CTransformable*>(pu->GetComponent(CompType::TransformableComp).get());
   float angleRotation = (cTransformable->rotation.y * PI) / 180.0;
   cTransformable->position.x -= cos(angleRotation) * cPuActual->speed * Constants::DELTA_TIME;
   cTransformable->position.z += sin(angleRotation) * cPuActual->speed * Constants::DELTA_TIME;

   cTransformable->rotation.z = Utils::GetAdjustedDegrees(cTransformable->rotation.z - 750 * Constants::DELTA_TIME);
}