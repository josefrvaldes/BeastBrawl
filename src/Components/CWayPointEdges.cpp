#include "CWayPointEdges.h"

CWayPointEdges::CWayPointEdges(){
    m_compType = CompType::WayPointEdgesComp;

    
}

CWayPointEdges::CWayPointEdges(int to, float cost){
    m_compType = CompType::WayPointEdgesComp;
    Edge e;
    e.to   = to;
    e.cost = cost;
    edges.push_back(e);
}

void CWayPointEdges::AddEdge(int to, float cost){
    Edge e;
    e.to   = to;
    e.cost = cost;
    edges.push_back(e);
}