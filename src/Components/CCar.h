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
    ~CCar(){};

    friend ostream &operator<<(ostream &out, CCar &c)
    {
        //out << "(" << c.GetX() << "," << c.GetY() << "," << c.GetZ() << ")";
        return out;
    }

    float speed = 0.0;
    float maxSpeed;
    float acceleration = 0.0;
    float reverseMaxSpeed = -6.0;
    float wheelRotation = 0.0;
    float friction;
    float slowDown;

};