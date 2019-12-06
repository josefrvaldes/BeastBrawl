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
    string texture = "wall.bmp";
    string mesh    = "media/ninja.b3d";
    uint32_t IdWheelTopLeft;
    uint32_t IdWheelTopRight;
    uint32_t IdWheelBottomLeft;
    uint32_t IdWheelBottomRight;

};