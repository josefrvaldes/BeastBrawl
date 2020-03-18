#include "CMesh.h"


// esto HAY que quitarlo, porque estamos guardando el triple de información de manera innecesaria.
// tenemos que asegurarnos de que todas las entidades con malla tengan 3
CMesh::CMesh(std::string pathCerca)
: mesh{pathCerca}
{
    m_compType = CompType::MeshComp;
}

CMesh::CMesh(std::string pathCerca, std::string pathMedio, std::string pathLejos)
: mesh{pathCerca, pathMedio, pathLejos}
{
    m_compType = CompType::MeshComp;
}