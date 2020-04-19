#include "SystemGameRules.h"

#include <Entities/CarAI.h>
#include <Components/CShield.h>
#include <Components/CTotem.h>
#include <Components/CNitro.h>
#include <Components/CClock.h>

#include <Facade/Render/RenderFacadeManager.h>



SystemGameRules::SystemGameRules(){

}

void SystemGameRules::UpdateGameRules(Entity& globalClock_) const{

    // UPDATE RELOJ GLOBAL
    auto cClock = static_cast<CClock*>(globalClock_.GetComponent(CompType::ClockComp).get());
    if(cClock->active) {
        cClock->accumulatedTime += duration_cast<milliseconds>(system_clock::now() - cClock->timeStart).count();
        cClock->timeStart = system_clock::now();
    }
    if(cClock->accumulatedTime/1000.0 > cClock->DURATION_TIME/1000.0){
        cout << "Se acabo el tiempo, nadie gana!!! \n";
        //Game::GetInstance()->SetState(State::ENDRACE);
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_ENDRACE});

    }


}



void SystemGameRules::UpdateRulesCarPowerUps(Entity& car_, Entity& totem_) const{
    auto cTotem = static_cast<CTotem*>(car_.GetComponent(CompType::TotemComp).get());
    if (cTotem->active) {
        cTotem->accumulatedTime += duration_cast<milliseconds>(system_clock::now() - cTotem->timeStart).count();
        cTotem->timeStart = system_clock::now();
        // lo pintamos encima del coche
        auto cTransformCar = static_cast<CTransformable *>(car_.GetComponent(CompType::TransformableComp).get());
        auto cTransformTotem = static_cast<CTransformable *>(totem_.GetComponent(CompType::TransformableComp).get());
            cTransformTotem->position.x = cTransformCar->position.x;
            cTransformTotem->position.z = cTransformCar->position.z;
            cTransformTotem->position.y = cTransformCar->position.y + 10.0f;
    }


    //Sonido reloj - ESTE EN REALIDAD SERIA EL RELOJ GLOBAL
    /*int time = (cTotem->DURATION_TIME/1000.0 - cTotem->accumulatedTime/1000.0);
    if ( cTotem->active && time == 4) {
        //cout << "PLAY" << endl;
        EventManager::GetInstance().AddEventMulti(Event{EventType::CLOCK});
    } else if(cTotem->active && (time > 4 || time < 0))  {
        //cout << "PARA" << endl;
        EventManager::GetInstance().AddEventMulti(Event{EventType::NOT_CLOCK});
    }*/

    if(cTotem->accumulatedTime/1000.0 > cTotem->DURATION_TIME/1000.0){
        cout << "Has ganado \n";
        //Game::GetInstance()->SetState(State::ENDRACE);
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_ENDRACE});

    }
    // Actualiza el componente nitro
    auto cNitro = static_cast<CNitro *>(car_.GetComponent(CompType::NitroComp).get());
    if(cNitro->activePowerUp==true && duration_cast<milliseconds>(system_clock::now() - cNitro->timeStart).count() > cNitro->durationTime){  // comprueba el tiempo desde que se lanzo
        cNitro->deactivePowerUp();
    }
    // Actualiza el componente escudo
    auto cShield = static_cast<CShield *>(car_.GetComponent(CompType::ShieldComp).get());
    if(cShield->activePowerUp==true && duration_cast<milliseconds>(system_clock::now() - cShield->timeStart).count() > cShield->durationTime){  // comprueba el tiempo desde que se lanzo
        cShield->deactivePowerUp();
    }
}
