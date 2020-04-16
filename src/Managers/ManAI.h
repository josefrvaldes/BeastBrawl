#pragma once

#include <memory>
#include <queue>
#include <vector>
#include "../Systems/SystemAI.h"


using namespace std;

struct SystemGameRules;


struct BehaviorRecord
{
    CarAI*      b_carAI;
    SystemAI*   b_sysAI;
    double      b_frecuency;
    double      b_pharse;
};


class ManAI {
   public:
    ManAI();
    ~ManAI();

    void addBehavior(CarAI* , SystemAI*, double frec, double phase);

    void Update();

   private:
    double frameActual {0};
    //unique_ptr<SystemGameRules> systemGameRules;
    std::vector<BehaviorRecord> behaviours;
    std::queue<BehaviorRecord>  runThese;  
};
