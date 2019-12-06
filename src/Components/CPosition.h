#pragma once

#include "Component.h"
#include <string>
#include <iostream>

using namespace std;

class CPosition : public Component
{
public:
    CPosition();
    CPosition(float x, float y, float z);
    ~CPosition(){};

    friend ostream &operator<<(ostream &out, CPosition &c)
    {
        out << "(" << c.x << "," << c.y << "," << c.z << ")";
        return out;
    }

    float x;
    float y;
    float z;
};
