#pragma once

#include "Component.h"
#include "../../include/glm/glm.hpp"

#include <string>
#include <iostream>
#include <glm/vec3.hpp>

using namespace std;
using namespace glm;
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
    
    
        void ApplyPhysics(float maxSpeed_, float acc_, float revMaxSpeed_, float slowDown_);
    
        friend ostream &operator<<(ostream &out, CCamera &c)
        {
            //out << "(" << c.GetX() << "," << c.GetY() << "," << c.GetZ() << ")";
            return out;
        }
    
        float tarX;
        float tarY;
        float tarZ;
    
        float rotExtraY;
        float incrementrotExtraY = 0.63;  // velocidad por iteracion que se gira la camara comprovar que es relativo al giro de la rueda
        float decrementrotExtraY = 0.95;  // velocidad por iteracion que se recupera la camara comprovar que es relativo al giro de la rueda
        float rotExtraCamera = 8.0;   // grados extra de la camara respecto al coche 
        CamType camType = CamType::NORMAL_CAM;        // Tipo de seguimiento

        float speed {0.0};
        float slowDown;
        float maxSpeed;
        float acceleration;
        float reverseMaxSpeed;

        glm::vec3 target;
//cTransformable->position.x
    private:

};