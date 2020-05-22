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

#include <chrono>
#include <iostream>
#include <string>

typedef std::chrono::high_resolution_clock Clock;

using namespace std;
using namespace chrono;

class CColliding : public Component {
   public:
    CColliding();
    CColliding(bool colliding_);

    bool colliding{false};
    time_point<system_clock> lastTimeCollided{system_clock::now()};
    //int64_t milis = duration_cast<milliseconds>(now - then).count();
};