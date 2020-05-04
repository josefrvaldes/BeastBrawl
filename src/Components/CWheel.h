#pragma once

#include "Component.h"

#include <string>
#include <iostream>
#include <memory>

using namespace std;

class CWheel : public Component
{
public:
    CWheel();
    CWheel(float _offsetX,float _offsetZ);
    ~CWheel(){};

    friend ostream &operator<<(ostream &out, CWheel &c)
    {
        //out << "(" << c.GetX() << "," << c.GetY() << "," << c.GetZ() << ")";
        return out;
    }

    float offsetX = 0.0;
    float offsetZ = 0.0;
    float rotY = 0.0;

    string meshTopLeft = "sharky_wheel1.obj";
    string meshTopRight = "sharky_wheel1.obj";
    string meshBottomLeft = "sharky_wheel1.obj";
    string meshBottomRight = "sharky_wheel1.obj";

    uint32_t IdWheelTopLeft;
    uint32_t IdWheelTopRight;
    uint32_t IdWheelBottomLeft;
    uint32_t IdWheelBottomRight;

};