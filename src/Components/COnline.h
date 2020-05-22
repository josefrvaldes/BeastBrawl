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
#include <vector>
#include "../Constants.h"
#include <time.h>
#include <chrono>
#include <unordered_map>

using namespace std;

class COnline : public Component{
public:
    COnline();
    ~COnline();

    uint16_t idClient = -1;
    vector<Constants::InputTypes> inputs;

    // estos 3 valores son para la sincronización del reloj
    unordered_map<uint16_t, int64_t> timeSyncClock;
    unordered_map<uint16_t, float> currentTurnout;
    unordered_map<uint16_t, int8_t> numMeasurements;
};