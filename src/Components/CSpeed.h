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

    float GetVX() { return m_vx; };
    float GetVY() { return m_vy; };
    float GetVZ() { return m_vz; };

    friend ostream &operator<<(ostream &out, CSpeed &c)
    {
        out << "(" << c.GetVX() << "," << c.GetVY() << "," << c.GetVZ() << ")";
        return out;
    }

private:
    float m_vx;
    float m_vy;
    float m_vz;
};
