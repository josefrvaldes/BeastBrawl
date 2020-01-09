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

#include "../Aliases.h"

using namespace std;

struct CarAI;
struct ManWayPoint;
struct Manager;
struct Data;

class SystemPathPlanning {
public:
    SystemPathPlanning();
    ~SystemPathPlanning(){};

    void Update(CarAI* carAI, ManWayPoint* graph, Manager* manNavMesh);
    void UpdateDijkstra(CarAI* carAI, ManWayPoint* graph, Manager* manNavMesh);
    stack<int> Dijkstra(ManWayPoint* graph, int start, int end);

   private:
    void SubscribeToEvents();
    void MoveToPowerUp(DataMap d);
    void ChangePosDestination(DataMap d);

};