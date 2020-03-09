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

class PhysicsAI {
public:
    PhysicsAI();
    ~PhysicsAI(){};
/*
    void Update(CarAI* carAI, ManWayPoint* graph);
    void UpdateCarPowerUps(Entity* carAI);
*/
};