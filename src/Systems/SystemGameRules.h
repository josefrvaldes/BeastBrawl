#pragma once


#include <vector>
#include <stack>
#include <iostream>
#include <cstdio>
#include <climits>
#include <algorithm>
#include <cstdlib>     /* srand, rand */
#include <unordered_map>
#include <glm/glm.hpp>

#include <cmath>
#include <memory>
using namespace std;

struct CarAI;
struct Entity;
struct ManWayPoint;

class SystemGameRules {
    public:
        SystemGameRules();
        SystemGameRules(float, float, float, float);
        ~SystemGameRules(){};

        /**
         * @return true o false dependiendo si la partida debe terminar o no
         */
        bool UpdateRulesCarPowerUps(Entity& car_, Entity& totem_) const;
        
        /**
         * @return true o false dependiendo si la partida debe terminar o no
         */
        bool UpdateGameRules(Entity& globalClock_) const;

        void UpdatePositionsMiniMap(const vector<shared_ptr<Entity>> &cars, const vector<shared_ptr<Entity>> &totems, unordered_map<uint16_t, glm::vec2> &positions, glm::vec2 &positionTotem);
        
        void RestartAllTimers(vector<shared_ptr<Entity>> entities, Entity &globalClock, int64_t timeStartPause);

    private:

        float xLeftMap { 0 };       float xRightMap { 0 };
        float yUpMap { 0 };         float yDownMap { 0 };

        float widthMap { 0 };       float heightMap { 0 };

};