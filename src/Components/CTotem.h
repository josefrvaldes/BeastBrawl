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

#include <glm/vec3.hpp>
#include <string>
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;
//using namespace glm;

class CTotem : public Component{
public:
    CTotem();
    ~CTotem();


    bool active = false;
    time_point<system_clock> timeStart;        // reloj que contabiliza, cada vez que tenemos el totem, el tiempo
    
    const int32_t SEGUNDOS;
    const int32_t DURATION_TIME;       // X seg, debes mantenerlo en tu poder
    
    int64_t accumulatedTime = 0;
    uint16_t positionRanking = 0;               //Posicion en la que vas de la carrera
    
    // son las fisicas de la entidad TOTEM (cuando el totem esta en el suelo)
    float MaxSpeed {500.0};
    float speed {500.0}; // lo inicializamos al valor del MaxSpeed para que cuando se cree no se mueva
    float aceleration {20.0};

};