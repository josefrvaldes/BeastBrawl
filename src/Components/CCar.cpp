#include "CCar.h"

CCar::CCar(float _maxSpeed, float _acceleration, float _carFriction, float _carSlowDown)
: maxSpeed(_maxSpeed), acceleration(_acceleration), friction(_carFriction), slowDown(_carSlowDown)
{
    m_compType = CompType::CarComp;
    previousPos = vec3(0.0f,0.0f,0.0f);
}
