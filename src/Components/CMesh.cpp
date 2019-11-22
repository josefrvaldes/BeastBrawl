#include "CMesh.h"

CMesh::CMesh(std::string path){
    m_compType = CompType::Mesh;
    m_mesh  = path;
}