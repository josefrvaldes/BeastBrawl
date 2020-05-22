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
 * @author Carlos de la Fuente Torres <delafuentetorresc@gmail.com>
 * 
 */
 
 
 #pragma once

#include <memory>
#include <queue>
#include <vector>
#include "../Systems/SystemAI.h"
#include <chrono>


using namespace std;

struct SystemGameRules;


struct BehaviorRecord
{
    CarAI*      b_carAI;
    SystemAI*   b_sysAI;
    int      b_frecuency;
    int      b_pharse;
    double      b_timeMax;
};


class ManAI {
   public:
    ManAI();
    ~ManAI(){};

    void addBehavior(CarAI* , SystemAI*, int frec, int phase, double timeMax);

    void Update();

   private:
    int frameActual {0};
    //unique_ptr<SystemGameRules> systemGameRules;
    std::vector<BehaviorRecord> behaviours;
    std::queue<BehaviorRecord>  runThese;  

    std::chrono::time_point<std::chrono::system_clock> timeStart;        // reloj que contabiliza el tiempo
    int64_t accumulatedTime;


    //cClock->accumulatedTime += duration_cast<milliseconds>(system_clock::now() - cClock->timeStart).count();
    //cClock->timeStart = system_clock::now();


};
