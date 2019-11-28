#pragma once

#include "Component.h"
#include <string>
#include <iostream>

using namespace std;

class CCar : public Component
{
public:
    CCar();
    CCar(float maxSpeed,float acceleration, float carFriction, float carSlowDown);
    ~CCar();

    float GetSpeed() { return m_speed; };
    float GetMaxSpeed() { return m_maxSpeed; };
    float GetAcceleration() { return m_acceleration; };
    float GetWheelRotation() { return m_wheelRotation; };
    float GetFriction() { return m_carFriction; };
    float GetSlowDown() { return m_carSlowDown; };

    void SetSpeed(float speed){ m_speed = speed;};
    void SetAcceleration(float acceleration){ m_acceleration = acceleration;};
    void SetWheelRotation(float wheelRotation){ m_wheelRotation = wheelRotation;};

    friend ostream &operator<<(ostream &out, CCar &c)
    {
        //out << "(" << c.GetX() << "," << c.GetY() << "," << c.GetZ() << ")";
        return out;
    }

private:
    float m_speed = 0;
    float m_maxSpeed;
    float m_acceleration = 0;
    float m_wheelRotation = 0;
    float m_carFriction;
    float m_carSlowDown;

};