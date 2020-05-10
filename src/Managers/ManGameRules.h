#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include <Entities/GameObject.h>
#include <GameValues.h>

using namespace std;

class Entity;
struct SystemGameRules;

class ManGameRules{
   public:
    ManGameRules();
    ~ManGameRules();

    const unordered_map<uint16_t, glm::vec2>& GetPositionsPlane()   { return positionsPlane; };
    const glm::vec2& GetPositionTotemPlane()                         { return positionTotemPlane; };

    void SubscribeToEvents();
    void RestartAllTimers(vector<shared_ptr<Entity>> entities, int64_t timeStartPause);
    void ResetClock();

    void CreateCurrentMap(MAP type);
    vector<shared_ptr<GameObject>> GetGrounds(){ return ground; }; 
    /**
     * @return true o false dependiendo si la partida debe terminar o no
     */
    bool Update(const vector<shared_ptr<Entity>> &cars, const vector<shared_ptr<Entity>> &totems);
    unique_ptr<Entity>& GetGlobalClock() { return globalClock; };

   private:
    void CreateMayanMap();
    void CreateGlobalClock( const uint32_t durationTime_ );
    unique_ptr<Entity> globalClock;
    unique_ptr<SystemGameRules> systemGameRules;

    unordered_map<uint16_t, glm::vec2> positionsPlane;
    glm::vec2 positionTotemPlane { glm::vec2(-1, -1) };


    vector<shared_ptr<GameObject>> ground;

};
