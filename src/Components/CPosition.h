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
    ~CPosition();

    float GetX() { return m_x; };
    float GetY() { return m_y; };
    float GetZ() { return m_z; };

    void SetX(float x) { m_x = x; };
    void SetY(float y) { m_y = y; };
    void SetZ(float z) { m_z = z; };

    friend ostream &operator<<(ostream &out, CPosition &c)
    {
        out << "(" << c.GetX() << "," << c.GetY() << "," << c.GetZ() << ")";
        return out;
    }

private:
    float m_x;
    float m_y;
    float m_z;
};
