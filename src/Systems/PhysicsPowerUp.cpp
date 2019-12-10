#include "PhysicsPowerUp.h"


PhysicsPowerUp::PhysicsPowerUp(){}

    //void init();
void PhysicsPowerUp::update(vector<shared_ptr<PowerUp>> _PowerUps){

    for (shared_ptr<PowerUp> pu : _PowerUps){
        std::cout << "QUE PASAAAAAA!!!!" << std::endl;

        type actualPowerUp = pu->getTypePowerUp();
        switch ( actualPowerUp ){
           case type::RoboJorobo:       updateRoboJorobo(pu.get());
              break;
           case type::SuperMegaNitro:   updateSuperMegaNitro(pu.get());
              break;
           case type::PudinDeFrambuesa: updatePudinDeFrambuesa(pu.get());
              break;
           case type::EscudoMerluzo:    updateEscudoMerluzo(pu.get());
              break;
           case type::TeleBanana:       updateTeleBanana(pu.get());
              break;
           case type::MelonMolon:       updateMelonMolon(pu.get());
              break;
           default:
              break;
        }
    }

}


void PhysicsPowerUp::updateRoboJorobo(PowerUp* pu){

}
void PhysicsPowerUp::updateSuperMegaNitro(PowerUp* pu){

}
void PhysicsPowerUp::updatePudinDeFrambuesa(PowerUp* pu){

}
void PhysicsPowerUp::updateEscudoMerluzo(PowerUp* pu){

}
void PhysicsPowerUp::updateTeleBanana(PowerUp* pu){

}
void PhysicsPowerUp::updateMelonMolon(PowerUp* pu){

}