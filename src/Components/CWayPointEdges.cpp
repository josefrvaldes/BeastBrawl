#include "CWayPointEdges.h"

CWayPointEdges::CWayPointEdges(){
    m_compType = CompType::WayPointEdgesComp;

    
}

void CWayPointEdges::AddEdge(int to, float cost){
    Edge e;
    e.to   = to;
    e.cost = cost;
    edges.push_back(e);
}