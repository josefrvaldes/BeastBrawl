#pragma once

#include "Component.h"
#include <string>
#include <iostream>

using namespace std;

class CTransformable : public Component
{
public:
    CTransformable();
    CTransformable(float posx, float posy, float posz, float rotx, float roty, float rotz, float scalex, float scaley, float scalez);
    ~CTransformable();

    friend ostream &operator<<(ostream &out, CTransformable &c)
    {
        //out << "(" << c.GetX() << "," << c.GetY() << "," << c.GetZ() << ")";
        return out;
    }

    float posX;
    float posY;
    float posZ;

    float rotX;
    float rotY;
    float rotZ;

    float scaleX;
    float scaleY;
    float scaleZ;
};