#include "PhysicsAI.h"

#include <Entities/CarAI.h>
#include <Components/CShield.h>
#include <Components/CTotem.h>
#include <Components/CNitro.h>
#include "../Entities/CarHuman.h"

#include <Facade/Render/RenderFacadeManager.h>



PhysicsAI::PhysicsAI(){

}


/*
void PhysicsAI::Update(CarAI* carAI, ManWayPoint* graph){
    UpdateCarPowerUps(carAI);
}


void PhysicsAI::UpdateCarPowerUps(Entity* carAI){
    auto cTotem = static_cast<CTotem*>(carAI->GetComponent(CompType::TotemComp).get());
    if(cTotem->active){
        cTotem->accumulatedTime +=  duration_cast<milliseconds>(system_clock::now() - cTotem->timeStart).count();
        cTotem->timeStart = system_clock::now();
    }

    if(cTotem->accumulatedTime/1000.0 > cTotem->durationTime/1000.0){
        cout << "Has ganado IA\n";
        //Game::GetInstance()->SetState(State::ENDRACE);
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_ENDRACE});

    }

    // Actualiza el componente nitro
    auto cNitro = static_cast<CNitro *>(carAI->GetComponent(CompType::NitroComp).get());
    if(cNitro->activePowerUp==true && duration_cast<milliseconds>(system_clock::now() - cNitro->timeStart).count() > cNitro->durationTime){  // comprueba el tiempo desde que se lanzo
        cNitro->deactivePowerUp();
    }

    // Actualiza el componente escudo
    auto cShield = static_cast<CShield *>(carAI->GetComponent(CompType::ShieldComp).get());
    if(cShield->activePowerUp==true && duration_cast<milliseconds>(system_clock::now() - cShield->timeStart).count() > cShield->durationTime){  // comprueba el tiempo desde que se lanzo
        cShield->deactivePowerUp();
    }
}*/

