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
#include <chrono>

#include <iostream>

enum class eventHUDType { NONE, STOLE, CATCH, LOSE };

class CEventHUD : public Component {
    public:
        CEventHUD();
        ~CEventHUD() = default;

        eventHUDType typeEvent { eventHUDType::NONE };
        std::string spriteTypeEvent { "media/loseHUD.png" };
        uint16_t characterEventHUD { 0 };
        std::chrono::time_point<std::chrono::system_clock> timeStart;        // reloj que contabiliza el tiempo que esta activo un eventhud
        const uint32_t DURATION_TIME {3};
        int64_t accumulatedTime { 0 };
};