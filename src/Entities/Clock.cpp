#include "Clock.h"

#include "../Components/CClock.h"

Clock::Clock(uint32_t durationTime_){
    shared_ptr<CClock> cClock = make_shared<CClock>(durationTime_);
    AddComponent(cClock);
}
