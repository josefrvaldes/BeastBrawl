#pragma once

#include "Component.h"
#include <string>
#include <iostream>

using namespace std;

class CCar : public Component
{
public:
    CCar();
    CCar(float speed, float maxSpeed, float acceleration, float wheelRotation, float carFriction, float carSlowDown);
    ~CCar();

    float GetSpeed() { return m_speed; };
    float GetMaxSpeed() { return m_maxSpeed; };
    float GetAcceleration() { return m_acceleration; };
    float GetWheelRotation() { return m_wheelRotation; };
    float GetCarFriction() { return m_carFriction; };
    float GetCarSlowDown() { return m_carSlowDown; };

    friend ostream &operator<<(ostream &out, CCar &c)
    {
        //out << "(" << c.GetX() << "," << c.GetY() << "," << c.GetZ() << ")";
        return out;
    }

private:
    float m_speed;
    float m_maxSpeed;
    float m_acceleration;
    float m_wheelRotation;
    float m_carFriction;
    float m_carSlowDown;

};