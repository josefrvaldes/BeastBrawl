#pragma once

#include <memory>
using namespace std;

struct SystemGameRules;


struct BehaviorRecord
{
    XXX                function;
    double      frecuency;
    double      pharse;
};


class ManAI {
   public:
    ManAI();
    ~ManAI();

    void Update();

   private:


    unique_ptr<SystemGameRules> systemGameRules;
};
