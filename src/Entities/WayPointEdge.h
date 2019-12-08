#pragma once

#include "Entity.h"
#include <memory>
#include <iostream>
using namespace std;

class WayPointEdge : public Entity{
    public:

    //WayPointEdge();
    WayPointEdge(int from, int to, double cost);
    ~WayPointEdge(){};

};