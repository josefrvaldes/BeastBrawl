#ifndef __POSITION_H__
#define __POSITION_H__

#include "component.h"

class CPosition : public Component
{
public:
    CPosition();
    ~CPosition();

    float GetX() { return m_x; };
    float GetY() { return m_y; };
    float GetZ() { return m_z; };

private:
    float m_x;
    float m_y;
    float m_z;
};

#endif