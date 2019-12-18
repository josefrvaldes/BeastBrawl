#include "PhysicsPowerUp.h"
#include "../Components/CPowerUp.h"


PhysicsPowerUp::PhysicsPowerUp(){

}

    //void init();
void PhysicsPowerUp::update(vector<shared_ptr<PowerUp>> _PowerUps){

    for (shared_ptr<PowerUp> pu : _PowerUps){
        //std::cout << "QUE PASAAAAAA!!!!" << std::endl;

        typeCPowerUp actualPowerUp = static_cast<CPowerUp*>(pu.get()->GetComponent(CompType::PowerUpComp).get())->typePowerUp;
        switch ( actualPowerUp ){
           case typeCPowerUp::PudinDeFrambuesa: updatePudinDeFrambuesa(pu.get());
              break;
           case typeCPowerUp::TeleBanana:       updateTeleBanana(pu.get());
              break;
           case typeCPowerUp::MelonMolon:       updateMelonMolon(pu.get());
              break;
           default:
              break;
        }
    }

}

// TODO: pequenya animacion del puding para cuando lo lancemos estando quietos, que no nos afecte
void PhysicsPowerUp::updatePudinDeFrambuesa(PowerUp* pu){

   time_point<system_clock> now = system_clock::now();

   auto cPuActual = static_cast<CPowerUp*>(pu->GetComponent(CompType::PowerUpComp).get());
   if(cPuActual->effectActive == false){
      if(duration_cast<milliseconds>(now - cPuActual->timeStart).count() > cPuActual->durationTime ){
            // A partir de aqui el powerUp hace efecto a cualquier Entidad coche del mapa
            cPuActual->effectActive = true;
            //std::cout << " AQUIIIII EMPEZAMOS REALMENTE" << std::endl;
      }
       // Movimiento
      auto cTransformable = static_cast<CTransformable*>(pu->GetComponent(CompType::TransformableComp).get());
      float angleRotation = (cTransformable->rotation.y * PI) / 180.0;
      cTransformable->position.x -= cos(angleRotation) * cPuActual->speed;
      cTransformable->position.z += sin(angleRotation) * cPuActual->speed;
      cPuActual->speed += cPuActual->deceleration;
      if(cPuActual->speed > 0)
         cPuActual->speed = 0; 
   }
}


void PhysicsPowerUp::updateTeleBanana(PowerUp* pu){
   time_point<system_clock> now = system_clock::now();

   auto cPuActual = static_cast<CPowerUp*>(pu->GetComponent(CompType::PowerUpComp).get());
   if(cPuActual->effectActive == false){
      if(duration_cast<milliseconds>(now - cPuActual->timeStart).count() > cPuActual->durationTime ){
            // A partir de aqui el powerUp hace efecto a cualquier Entidad coche del mapa
            cPuActual->effectActive = true;
            //std::cout << " AQUIIIII EMPEZAMOS REALMENTE" << std::endl;
      }
   }
   auto cTargetCar = static_cast<CTargetEntity*>(pu->GetComponent(CompType::TargetEntityComp).get());
   auto cTransformable = static_cast<CTransformable*>(pu->GetComponent(CompType::TransformableComp).get());
   // Vector
   float vectorX = cTargetCar->cTransTarget->position.x - cTransformable->position.x;
   float vectorZ = cTargetCar->cTransTarget->position.z - cTransformable->position.z;
   // divisor unitario
   float divisorUnitario = sqrt((vectorX*vectorX) + (vectorZ*vectorZ));
   // Movimiento
   cTransformable->position.x += (vectorX/divisorUnitario) * cPuActual->speed;
   cTransformable->position.z += (vectorZ/divisorUnitario) * cPuActual->speed;
}




void PhysicsPowerUp::updateMelonMolon(PowerUp* pu){
   time_point<system_clock> now = system_clock::now();

   auto cPuActual = static_cast<CPowerUp*>(pu->GetComponent(CompType::PowerUpComp).get());
   if(cPuActual->effectActive == false){
      if(duration_cast<milliseconds>(now - cPuActual->timeStart).count() > cPuActual->durationTime ){
            // A partir de aqui el powerUp hace efecto a cualquier Entidad coche del mapa
            cPuActual->effectActive = true;
            //std::cout << " AQUIIIII EMPEZAMOS REALMENTE" << std::endl;
      }
   }
   // Movimiento
   auto cTransformable = static_cast<CTransformable*>(pu->GetComponent(CompType::TransformableComp).get());
   float angleRotation = (cTransformable->rotation.y * PI) / 180.0;
   cTransformable->position.x -= cos(angleRotation) * cPuActual->speed;
   cTransformable->position.z += sin(angleRotation) * cPuActual->speed;
}