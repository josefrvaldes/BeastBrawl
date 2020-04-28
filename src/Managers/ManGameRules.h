#pragma once

#include <memory>
#include <vector>

using namespace std;

class Entity;
struct SystemGameRules;

class ManGameRules{
   public:
    ManGameRules( const uint32_t durationTime_);
    ~ManGameRules();

    void SubscribeToEvents();
    void RestartAllTimers(vector<shared_ptr<Entity>> entities);
    void ResetClock();

    /**
     * @return true o false dependiendo si la partida debe terminar o no
     */
    bool Update();
    unique_ptr<Entity>& GetGlobalClock() { return globalClock; };

   private:
    void CreateGlobalClock( const uint32_t durationTime_ );
    unique_ptr<Entity> globalClock;
    unique_ptr<SystemGameRules> systemGameRules;
};
