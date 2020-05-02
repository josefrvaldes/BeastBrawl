#pragma once


#include <stack>
#include <Aliases.h>

#include "SystemAI.h"

using namespace std;

struct CarAI;
struct ManWayPoint;
struct ManNavMesh;
struct Manager;
struct Data;


class SystemPathPlanning : public SystemAI {
public:
    SystemPathPlanning();
    ~SystemPathPlanning();

    void update(CarAI* actualCar) override;

    void AddManager(Manager &e);

    void UpdateDijkstra(CarAI* carAI, ManWayPoint* graph, ManNavMesh* manNavMesh);
    stack<int> Dijkstra(ManWayPoint* graph, const uint16_t start, const uint16_t end);

    int getFrecuency(){ return frec; };

   private:
    void SubscribeToEvents();
    void CalculatePathToNavMesh(DataMap* d);
    void ChangePosDestination(DataMap* d);
    void MoveRandomPowerUp(DataMap* d);
    void InitMapGraph(ManWayPoint* _graph);
    void CleanBrainAI(CarAI* carAI);
    float** graph;
    int graphSize = 0;
    bool graphCreated = false;

    vector<Manager *> managers;
    int frec {2};
};