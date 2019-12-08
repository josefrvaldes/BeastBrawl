#include "CWayPointEdge.h"

CWayPointEdge::CWayPointEdge(int from, int to, double cost){
    m_compType = CompType::WayPointEdgeComp;

    m_from = from;
    m_to   = to;
    m_cost = cost;
}