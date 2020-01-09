#pragma once


#include <vector>
#include <stack>
#include <iostream>
#include <stdio.h>
#include <limits.h>
#include <algorithm>
#include <stdlib.h>     /* srand, rand */

#include <math.h>
#include <memory>
using namespace std;

struct CarAI;
struct ManWayPoint;

class PhysicsAI {
public:
    PhysicsAI();
    ~PhysicsAI(){};

    void Update(CarAI* carAI, ManWayPoint* graph);
    void UpdateCarPowerUps(CarAI* carAI);

};