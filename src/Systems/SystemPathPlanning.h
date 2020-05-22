/**
 * Beast Brawl
 * Game created as a final project of the Multimedia Engineering Degree in the University of Alicante.
 * Made by Clover Games Studio, with members 
 * Carlos de la Fuente Torres delafuentetorresc@gmail.com,
 * Antonio Jose Martinez Garcia https://www.linkedin.com/in/antonio-jose-martinez-garcia/,
 * Jesús Mas Carretero jmasc03@gmail.com, 
 * Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/, 
 * Rubén Rubio Martínez https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/, 
 * and Jose Valdés Sirvent https://www.linkedin.com/in/jose-f-valdés-sirvent-6058b5a5/ github -> josefrvaldes
 * 
 * 
 * @author Clover Games Studio
 * 
 */
 
 
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
    float** graph = nullptr;
    int graphSize = 0;
    bool graphCreated = false;

    vector<Manager *> managers;
    int frec {2};
};