#pragma once

#include "Component.h"

#include <string>
#include <iostream>
#include <vector>

using namespace std;

class CNavMesh : public Component
{
public:
    CNavMesh(vector<int> _waypoints);
    CNavMesh();
    ~CNavMesh(){};

    void ResetNumIds(){numIdsNavMesh = 0;}

    uint16_t id; //Id del navmesh
    vector<int> waypoints; // Waypoints que tiene el navmesh

private:
    static uint16_t numIdsNavMesh;  //Lleva la cuenta del id de navMesh
};
