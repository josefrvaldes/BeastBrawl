#include "WayPointEdge.h"
#include "../Components/CWayPointEdge.h"


WayPointEdge::WayPointEdge(int from, int to, double cost)
{
    shared_ptr<CWayPointEdge> cWayPointEdge = make_shared<CWayPointEdge>(from, to, cost);
    AddComponent(cWayPointEdge);
}

