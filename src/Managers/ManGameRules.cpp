#include "ManGameRules.h"
#include "../Entities/Clock.h"
#include "../Components/CClock.h"
#include "../Systems/SystemGameRules.h"
#include <iostream>


ManGameRules::ManGameRules( const uint32_t durationTime_) {
    // creamos el reloj global
    systemGameRules = make_unique<SystemGameRules>();
    CreateGlobalClock(durationTime_);

}
void ManGameRules::CreateGlobalClock( const uint32_t durationTime_) {
    globalClock = make_unique<Clock>(durationTime_);

    auto cClock = static_cast<CClock*>(globalClock->GetComponent(CompType::ClockComp).get());
    cClock->active = true;
    cClock->timeStart = system_clock::now();
}

ManGameRules::~ManGameRules() {
    std::cout << "Llamando al destructor de ManGameRules" << std::endl;
}

void ManGameRules::Update(){
    systemGameRules->UpdateGameRules( *(globalClock.get()) );

}

void ManGameRules::SubscribeToEvents() {

}