#include "CNavMesh.h"

uint16_t CNavMesh::numIdsNavMesh = 0;


CNavMesh::CNavMesh(){
}



CNavMesh::CNavMesh(vector<int> _waypoints) {
    m_compType = CompType::NavMeshComp;
    id = numIdsNavMesh;
    waypoints = _waypoints;

    numIdsNavMesh++;
}

