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
struct Manager;

class PhysicsAI {
public:
    PhysicsAI();
    ~PhysicsAI(){};

    void Update(CarAI* carAI, ManWayPoint* graph, Manager* manNavMesh);
    void UpdateCarPowerUps(CarAI* carAI);
    void UpdateDijkstra(CarAI* carAI, ManWayPoint* graph, Manager* manNavMesh);
    stack<int> Dijkstra(ManWayPoint* graph, int start, int end);



protected:
private:

};