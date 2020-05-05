#include "ManGameRules.h"
#include "../Entities/Clock.h"
#include "../Components/CClock.h"
#include "../Systems/SystemGameRules.h"
#include <GameValues.h>
#include <iostream>


ManGameRules::ManGameRules() {
    // creamos el reloj global
    systemGameRules = make_unique<SystemGameRules>();
    // CreateGlobalClock(15);
    
    CreateGlobalClock(GameValues::GetInstance()->GetGameTime());

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

bool ManGameRules::Update(const vector<shared_ptr<Entity>> &cars, const vector<shared_ptr<Entity>> &totems){
    //cout << "HOLAAAA" << endl;
    systemGameRules->UpdatePositionsMiniMap(cars, totems, positionsPlane, positionTotemPlane);
    return systemGameRules->UpdateGameRules( *(globalClock.get()) );
}

void ManGameRules::RestartAllTimers(vector<shared_ptr<Entity>> entities, int64_t timeStartPause) {
    systemGameRules->RestartAllTimers(entities, *(globalClock.get()), timeStartPause);
}

void ManGameRules::SubscribeToEvents() {

}