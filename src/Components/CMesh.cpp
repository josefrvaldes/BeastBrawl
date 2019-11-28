#include "CMesh.h"

CMesh::CMesh(std::string path)
: m_mesh(path)
{
    m_compType = CompType::MeshComp;
}