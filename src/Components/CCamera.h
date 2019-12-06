#pragma once

#include "Component.h"
#include <string>
#include <iostream>

using namespace std;

class CCamera : public Component
{
public:
    CCamera();
    CCamera(float _tarx, float _tary, float _tarz, float _rotExtraY);
    ~CCamera(){};


    friend ostream &operator<<(ostream &out, CCamera &c)
    {
        //out << "(" << c.GetX() << "," << c.GetY() << "," << c.GetZ() << ")";
        return out;
    }

    float tarX;
    float tarY;
    float tarZ;

    float rotExtraY;
};