#pragma once

#include "Component.h"

#include <string>
#include <iostream>
#include "../../include/glm/vec3.hpp"
#include <chrono>


using namespace std;
using namespace glm;
using namespace chrono;

enum class SkidState{DISABLED, SKID_START, SKID_TO_LEFT, SKID_TO_RIGHT, SKID_LEFT, SKID_RIGHT, SKID_RECOVER_RIGHT, SKID_RECOVER_LEFT};

class CCar : public Component
{
public:
    CCar();
    CCar(float maxSpeed,float acceleration, float carFriction, float carSlowDown);
    ~CCar(){};

    friend ostream &operator<<(ostream &out, CCar &c)
    {
        //out << "(" << c.GetX() << "," << c.GetY() << "," << c.GetZ() << ")";
        return out;
    }

    float speed = 0.0;
    float maxSpeed;
    float acceleration = 0.0;
    float reverseMaxSpeed = -60.0;
    float wheelRotation = 0.0;
    float incrementWheelRotation = 0.5;  // velocidad por iteracion que se gira la rueda
    float decrementWheelRotation = 0.7;  // velocidad por iteracion que se recupera la rueda
    float maxWheelRotation = 5.0;
    float friction;
    float slowDown;


    // Derrape
    time_point<system_clock> skidStart;
    const int64_t skidActivationTime = 700;       // 0.7 segundos para activar el derrape
    const int64_t skidAnimationTime = 1000;       // 1 segundo de animacion
    SkidState skidState;
    float skidRotation = 0.0;                     


    float skidAcc = 1.0;        // To-Do: multiplicar por el deltaTime los periodos para ajustarlos al framerate
    float skidDeg;              // giro en el derrape
    float skidDegL = -35.0;  
    float skidDegR = 35.0;   



};