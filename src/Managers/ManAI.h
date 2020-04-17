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
    float      b_frecuency;
    float      b_pharse;
    double      b_timeMax;
};


class ManAI {
   public:
    ManAI();
    ~ManAI(){};

    void addBehavior(CarAI* , SystemAI*, float frec, float phase);

    void Update();

   private:
    double frameActual {0};
    //unique_ptr<SystemGameRules> systemGameRules;
    std::vector<BehaviorRecord> behaviours;
    std::queue<BehaviorRecord>  runThese;  

    std::chrono::time_point<std::chrono::system_clock> timeStart;        // reloj que contabiliza el tiempo
    int64_t accumulatedTime;


    //cClock->accumulatedTime += duration_cast<milliseconds>(system_clock::now() - cClock->timeStart).count();
    //cClock->timeStart = system_clock::now();


};
