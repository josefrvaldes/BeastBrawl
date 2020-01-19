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
    float reverseMaxSpeed = -60.0;
    float wheelRotation = 0.0;
    float incrementWheelRotation = 0.5;  // velocidad por iteracion que se gira la rueda
    float decrementWheelRotation = 0.7;  // velocidad por iteracion que se recupera la rueda
    float maxWheelRotation = 10;
    float friction;
    float slowDown;


};