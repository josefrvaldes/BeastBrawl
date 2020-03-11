#include "Clock.h"

#include "../Components/CClock.h"

Clock::Clock(){
    shared_ptr<CClock> cClock = make_shared<CClock>();
    AddComponent(cClock);
}
