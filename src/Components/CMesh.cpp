#include "CMesh.h"

CMesh::CMesh(std::string path)
: mesh(path)
{
    m_compType = CompType::MeshComp;
}