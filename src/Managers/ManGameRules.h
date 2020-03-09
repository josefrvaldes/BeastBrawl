#pragma once

#include <memory>

using namespace std;

class Entity;

class ManGameRules{
   public:
    ManGameRules();
    ~ManGameRules();

    void SubscribeToEvents();

   private:
    void CreateGlobalClock();
    unique_ptr<Entity> globalClock;
};
