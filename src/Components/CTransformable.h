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

    float GetPosX() { return m_posx; };
    float GetPosY() { return m_posy; };
    float GetPosZ() { return m_posz; };

    float GetRotX() { return m_rotx; };
    float GetRotY() { return m_roty; };
    float GetRotZ() { return m_rotz; };

    float GetScaleX() { return m_scalex; };
    float GetScaleY() { return m_scaley; };
    float GetScaleZ() { return m_scalez; };

    void SetPosX(float x) { m_posx = x; };
    void SetPosY(float y) { m_posy = y; };
    void SetPosZ(float z) { m_posz = z; };

    void SetRotX(float x) { m_rotx = x; };
    void SetRotY(float y) { m_roty = y; };
    void SetRotZ(float z) { m_rotz = z; };

    void SetScaleX(float x) { m_scalex = x; };
    void SetScaleY(float y) { m_scaley = y; };
    void SetScaleZ(float z) { m_scalez = z; };

    friend ostream &operator<<(ostream &out, CTransformable &c)
    {
        //out << "(" << c.GetX() << "," << c.GetY() << "," << c.GetZ() << ")";
        return out;
    }

private:
    float m_posx;
    float m_posy;
    float m_posz;

    float m_rotx;
    float m_roty;
    float m_rotz;

    float m_scalex;
    float m_scaley;
    float m_scalez;
};