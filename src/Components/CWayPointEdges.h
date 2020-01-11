#pragma once

#include "../../include/glm/vec3.hpp"

#include "Component.h"
#include <string>
#include <iostream>
#include <vector>

using namespace std;
//using namespace glm;
struct Edge{
    int to;
    float cost;
};

class CWayPointEdges : public Component
{
public:
    CWayPointEdges();
    CWayPointEdges(int to, float cost);
    void AddEdge(int to, float cost);
    ~CWayPointEdges(){};

    vector<Edge> edges;
    //int m_from;
    //int m_to;
    //double m_cost;
};
