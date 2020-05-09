#include "CCar.h"

CCar::CCar(mainCharacter _pj, float _weight, float _maxSpeed, float _acceleration)
: character(_pj), weight(_weight), maxSpeed(_maxSpeed), acceleration(_acceleration), skidState(SkidState::DISABLED)
{
    m_compType = CompType::CarComp;
    skidStart = system_clock::now();
}
 


// To-Do: temporal hasta que se ajuste el derrape
CCar::CCar(mainCharacter _pj, float _weight, float _maxSpeed, float _acceleration, float maxWheelRot)
: character(_pj), weight(_weight), maxSpeed(_maxSpeed), acceleration(_acceleration), skidState(SkidState::DISABLED), maxWheelRotation(maxWheelRot)
{
    m_compType = CompType::CarComp;
    skidStart = system_clock::now();
}

