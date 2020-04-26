#include "ManGameRules.h"
#include "../Entities/Clock.h"
#include "../Components/CClock.h"
#include "../Systems/SystemGameRules.h"
#include <iostream>


ManGameRules::ManGameRules( const uint32_t durationTime_) {
    // creamos el reloj global
    systemGameRules = make_unique<SystemGameRules>();
    // CreateGlobalClock(durationTime_);
    CreateGlobalClock(5);

}
void ManGameRules::CreateGlobalClock( const uint32_t durationTime_) {
    globalClock = make_unique<Clock>(durationTime_);

    auto cClock = static_cast<CClock*>(globalClock->GetComponent(CompType::ClockComp).get());
    cClock->active = true;
    cClock->timeStart = system_clock::now();
}

void ManGameRules::ResetClock() {
    auto cClock = static_cast<CClock*>(globalClock->GetComponent(CompType::ClockComp).get());
    cClock->active = true;
    cClock->timeStart = system_clock::now();
}

ManGameRules::~ManGameRules() {
}

bool ManGameRules::Update(){
    return systemGameRules->UpdateGameRules( *(globalClock.get()) );
}

void ManGameRules::RestartAllTimers(vector<shared_ptr<Entity>> entities) {
    systemGameRules->RestartAllTimers(entities, *(globalClock.get()));
}

void ManGameRules::SubscribeToEvents() {

}