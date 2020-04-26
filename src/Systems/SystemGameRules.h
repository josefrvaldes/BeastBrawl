#pragma once


#include <vector>
#include <stack>
#include <iostream>
#include <cstdio>
#include <climits>
#include <algorithm>
#include <cstdlib>     /* srand, rand */

#include <cmath>
#include <memory>
using namespace std;

struct CarAI;
struct Entity;
struct ManWayPoint;

class SystemGameRules {
public:
    SystemGameRules();
    ~SystemGameRules(){};

    /**
     * @return true o false dependiendo si la partida debe terminar o no
     */
    bool UpdateRulesCarPowerUps(Entity& car_, Entity& totem_) const;
    
    /**
     * @return true o false dependiendo si la partida debe terminar o no
     */
    bool UpdateGameRules(Entity& globalClock_) const;
    
    void RestartAllTimers(vector<shared_ptr<Entity>> entities, Entity &globalClock);

};