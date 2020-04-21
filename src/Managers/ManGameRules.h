#pragma once

#include <memory>

using namespace std;

class Entity;
struct SystemGameRules;

class ManGameRules{
   public:
    ManGameRules( const uint32_t durationTime_);
    ~ManGameRules();

    void SubscribeToEvents();
    void Update();
    unique_ptr<Entity>& GetGlobalClock() { return globalClock; };

   private:
    void CreateGlobalClock( const uint32_t durationTime_ );
    unique_ptr<Entity> globalClock;
    unique_ptr<SystemGameRules> systemGameRules;
};
