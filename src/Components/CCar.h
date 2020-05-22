/**
 * Beast Brawl
 * Game created as a final project of the Multimedia Engineering Degree in the University of Alicante.
 * Made by Clover Games Studio, with members 
 * Carlos de la Fuente Torres delafuentetorresc@gmail.com,
 * Antonio Jose Martinez Garcia https://www.linkedin.com/in/antonio-jose-martinez-garcia/,
 * Jesús Mas Carretero jmasc03@gmail.com, 
 * Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/, 
 * Rubén Rubio Martínez https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/, 
 * and Jose Valdés Sirvent https://www.linkedin.com/in/jose-f-valdés-sirvent-6058b5a5/ github -> josefrvaldes
 * 
 * 
 * @author Clover Games Studio
 * 
 */
 
 
 #pragma once

#include "Component.h"

#include <string>
#include <iostream>
#include "../../include/glm/vec3.hpp"
#include "../Constants.h"
#include <chrono>


using namespace std;
using namespace glm;
using namespace std::chrono;

enum class SkidState{DISABLED, SKID_START, SKID_TO_LEFT, SKID_TO_RIGHT, SKID_LEFT, SKID_RIGHT, SKID_RECOVER_RIGHT, SKID_RECOVER_LEFT};
enum class mainCharacter {PENGUIN, TIGER, SHARK, GORILLA, DRAGON, OCTOPUS};

class CCar : public Component
{
public:
    CCar(mainCharacter);
    CCar(mainCharacter, float weight, float maxSpeed,float acceleration);
    CCar(mainCharacter, float weight, float maxSpeed,float acceleration, float maxWheelRot);
    ~CCar(){};

    
    //PERSONAJE
    mainCharacter character { mainCharacter::PENGUIN };
    
    const float weight;
    float speed {0.0};
    const float maxSpeed;
    const float acceleration;

    const float reverseMaxSpeed {-85.0};
    float wheelRotation {0.0};
    const float incrementWheelRotation {0.5};  // velocidad por iteracion que se gira la rueda
    const float decrementWheelRotation {0.75};  // velocidad por iteracion que se recupera la rueda
    float maxWheelRotation {6.3};
    const float friction {1.0};
    const float slowDown {3.5};   // para todos igual


    // Derrape
    time_point<system_clock> skidStart;
    const int64_t skidActivationTime {700};       // 0.7 segundos para activar el derrape
    const int64_t skidAnimationTime {1000};       // 1 segundo de animacion
    SkidState skidState;
    float skidRotation {0.0};                     


    float skidAcc {60 * Constants::DELTA_TIME};
    float skidRecoverAcc {120 * Constants::DELTA_TIME};
    float skidDeg;              // giro en el derrape
    float skidDegL {-35.0};  
    float skidDegR {35.0};   
};