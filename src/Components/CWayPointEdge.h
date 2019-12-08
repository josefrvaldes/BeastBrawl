#pragma once

#include "../../lib/glm/vec3.hpp"

#include "Component.h"
#include <string>
#include <iostream>

using namespace std;
//using namespace glm;

class CWayPointEdge : public Component
{
public:
    CWayPointEdge();
    CWayPointEdge(int from, int to, double cost);
    ~CWayPointEdge(){};

    // friend ostream &operator<<(ostream &out, CWayPointEdge &c)
    // {
    //     //cout << "(" << c.position.x << "," << c.position.y << "," << c.position.z << ")";
    //     //return out;
    // }

    int m_from;
    int m_to;
    double m_cost;
};
