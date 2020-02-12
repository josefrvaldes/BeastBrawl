#pragma once

#include "Component.h"

#include <string>
#include <iostream>

using namespace std;
enum CamType{
    NORMAL_CAM,
    INVERTED_CAM,
    TOTEM_CAM
};
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
    float rotExtraCamera = 5.0;   // grados extra de la camara respecto al coche 
    CamType camType = CamType::NORMAL_CAM;        // Tipo de seguimiento


};