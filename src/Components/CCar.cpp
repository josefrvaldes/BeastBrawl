#include "CCar.h"

CCar::CCar(mainCharacter _pj, float _maxSpeed, float _acceleration, float _carFriction, float _carSlowDown)
: character(_pj), maxSpeed(_maxSpeed), acceleration(_acceleration), friction(_carFriction), slowDown(_carSlowDown), skidState(SkidState::DISABLED)
{
    m_compType = CompType::CarComp;
    skidStart = system_clock::now();
}
 


// To-Do: temporal hasta que se ajuste el derrape
CCar::CCar(mainCharacter _pj, float _maxSpeed, float _acceleration, float _carFriction, float _carSlowDown, float maxWheelRot)
: character(_pj), maxSpeed(_maxSpeed), acceleration(_acceleration), friction(_carFriction), slowDown(_carSlowDown), skidState(SkidState::DISABLED)
{
    m_compType = CompType::CarComp;
    skidStart = system_clock::now();
    maxWheelRotation = maxWheelRot;
}
 
 
