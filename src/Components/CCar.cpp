#include "CCar.h"

CCar::CCar(float maxSpeed, float acceleration, float carFriction, float carSlowDown)
: m_maxSpeed(maxSpeed),m_acceleration(acceleration) , m_carFriction(carFriction), m_carSlowDown(carSlowDown)
{
    m_compType = CompType::CarComp;
}
