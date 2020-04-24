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

    //Sonido reloj
    int time = cClock->DURATION_TIME/1000 - cClock->accumulatedTime/1000;
    if (time == 30) {
        EventManager::GetInstance().AddEventMulti(Event{EventType::CLOCK});
    } else if (time == 0 ) {
        EventManager::GetInstance().AddEventMulti(Event{EventType::NOT_CLOCK});
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


void SystemGameRules::RestartAllTimers(vector<shared_ptr<Entity>> entities, Entity &globalClock_) {
    cout << "Estamos reseteando los timers" << endl;
    auto cClock = static_cast<CClock*>(globalClock_.GetComponent(CompType::ClockComp).get());
    if(cClock->active) 
        cClock->timeStart = system_clock::now();

    for(auto e : entities) {
        auto cTotem = static_cast<CTotem*>(e->GetComponent(CompType::TotemComp).get());
        if(cTotem->active)
            cTotem->timeStart = system_clock::now();

        bool hasNitro = e->HasComponent(CompType::NitroComp);
        if(hasNitro) {
            auto cNitro = static_cast<CNitro*>(e->GetComponent(CompType::NitroComp).get());
            if(cNitro->activePowerUp)
                cNitro->timeStart = system_clock::now();
        }

        bool hasShield = e->HasComponent(CompType::NitroComp);
        if(hasShield) {
            auto cShield = static_cast<CShield*>(e->GetComponent(CompType::ShieldComp).get());
            if(cShield->activePowerUp)
                cShield->timeStart = system_clock::now();
        }
    }
}