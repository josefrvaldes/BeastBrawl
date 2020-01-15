#include "PhysicsAI.h"

#include "../Entities/Car.h"
#include "../Entities/CarAI.h"
#include "../Entities/WayPoint.h"


#include "../Components/CPowerUp.h"
#include "../Components/CShield.h"
#include "../Components/CTotem.h"
#include "../Components/CRoboJorobo.h"
#include "../Components/CNitro.h"
#include "../Components/CPath.h"
#include "../Components/CWayPointEdges.h"

#include "../Facade/Render/RenderFacadeManager.h"
#include "../Game.h"
#include "../Managers/Manager.h"
#include "../Managers/ManPowerUp.h"
#include "../Managers/ManBoxPowerUp.h"
#include "../Managers/ManTotem.h"
#include "../Managers/ManWayPoint.h"



PhysicsAI::PhysicsAI(){

}



void PhysicsAI::Update(CarAI* carAI, ManWayPoint* graph){
    UpdateCarPowerUps(carAI);
}


void PhysicsAI::UpdateCarPowerUps(CarAI* carAI){
    auto cTotem = static_cast<CTotem*>(carAI->GetComponent(CompType::TotemComp).get());
    if(cTotem->active){
        cTotem->accumulatedTime +=  duration_cast<milliseconds>(system_clock::now() - cTotem->timeStart).count();
        cTotem->timeStart = system_clock::now();
    }

    if(cTotem->accumulatedTime/1000.0 > cTotem->durationTime/1000.0){
        cout << "Has ganado\n";
        Game::GetInstance()->SetState(State::ENDRACE);
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
}

