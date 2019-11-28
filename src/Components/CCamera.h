#pragma once

#include "Component.h"
#include <string>
#include <iostream>

using namespace std;

class CCamera : public Component
{
public:
    CCamera();
    CCamera(float tarx, float tary, float tarz, float rotExtraY);
    ~CCamera();

    float GetTarX() { return m_tarx; };
    float GetTarY() { return m_tary; };
    float GetTarZ() { return m_tarz; };

    float GetRotExtraY() { return m_rotExtraY; };

    void SetTarX(float tarX) {m_tarx = tarX;};
    void SetTarY(float tarY) {m_tary = tarY;};
    void SetTarZ(float tarZ) {m_tarz = tarZ;};

    void SetRotExtraY(float rotExtraY) { m_rotExtraY = rotExtraY; };



    friend ostream &operator<<(ostream &out, CCamera &c)
    {
        //out << "(" << c.GetX() << "," << c.GetY() << "," << c.GetZ() << ")";
        return out;
    }

private:
    float m_tarx;
    float m_tary;
    float m_tarz;

    float m_rotExtraY;
};