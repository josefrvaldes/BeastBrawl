#include "CCar.h"

CCar::CCar(float speed, float maxSpeed, float acceleration, float wheelRotation, float carFriction, float carSlowDown){
    m_compType = CompType::CarValues;
    m_speed = speed;
    m_maxSpeed = maxSpeed;
    m_acceleration = acceleration;
    m_wheelRotation = wheelRotation;
    m_carFriction = carFriction;
    m_carSlowDown = carFriction;


}
