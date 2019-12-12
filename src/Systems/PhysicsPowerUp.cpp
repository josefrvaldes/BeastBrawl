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
           case typeCPowerUp::RoboJorobo:       updateRoboJorobo(pu.get());
              break;
           case typeCPowerUp::SuperMegaNitro:   updateSuperMegaNitro(pu.get());
              break;
           case typeCPowerUp::PudinDeFrambuesa: updatePudinDeFrambuesa(pu.get());
              break;
           case typeCPowerUp::EscudoMerluzo:    updateEscudoMerluzo(pu.get());
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


void PhysicsPowerUp::updateRoboJorobo(PowerUp* pu){
   std::cout << " EY TIO, UPDATE DE ROBOJOROBO" << std::endl;
}
void PhysicsPowerUp::updateSuperMegaNitro(PowerUp* pu){
   std::cout << " EY TIO, UPDATE DE SUPER MEGA NITRO" << std::endl;
}
void PhysicsPowerUp::updatePudinDeFrambuesa(PowerUp* pu){
   std::cout << " EY TIO, UPDATE DE PUDIN FRAMBUESA" << std::endl;
}
void PhysicsPowerUp::updateEscudoMerluzo(PowerUp* pu){
   std::cout << " EY TIO, UPDATE DE ESCUDO MERLUZO" << std::endl;
}
void PhysicsPowerUp::updateTeleBanana(PowerUp* pu){
   std::cout << " EY TIO, UPDATE DE TELEBANANA" << std::endl;
}
void PhysicsPowerUp::updateMelonMolon(PowerUp* pu){
   std::cout << " EY TIO, UPDATE DE MELONMOLON" << std::endl;
}