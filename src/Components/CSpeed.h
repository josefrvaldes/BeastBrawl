#pragma once

#include "Component.h"
#include <string>
#include <iostream>

using namespace std;

class CSpeed : public Component
{
public:
    CSpeed();
    CSpeed(float vx, float vy, float vz);
    ~CSpeed();

    friend ostream &operator<<(ostream &out, CSpeed &c)
    {
        out << "(" << c.x << "," << c.y << "," << c.z << ")";
        return out;
    }

    float x;
    float y;
    float z;
};
