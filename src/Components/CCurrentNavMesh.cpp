#include "CCurrentNavMesh.h"

CCurrentNavMesh::CCurrentNavMesh(int r_currentNavMesh) {
    m_compType = CompType::CurrentNavMeshComp;

    currentNavMesh = r_currentNavMesh;
}

