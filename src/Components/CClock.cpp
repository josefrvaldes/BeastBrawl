#include "CClock.h"


// el tiempo debe estar en segundos
CClock::CClock(const uint32_t durationGame) : active{false}, DURATION_TIME{ 1000*durationGame }, accumulatedTime{0} {
    m_compType = CompType::ClockComp; 
}