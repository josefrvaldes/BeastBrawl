#include "CCar.h"

CCar::CCar(float _maxSpeed, float _acceleration, float _carFriction, float _carSlowDown)
: maxSpeed(_maxSpeed), acceleration(_acceleration), friction(_carFriction), slowDown(_carSlowDown), skidState(SkidState::DISABLED)
{
    m_compType = CompType::CarComp;
    skidStart = system_clock::now();
}
 