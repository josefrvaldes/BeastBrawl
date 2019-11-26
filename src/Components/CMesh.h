#pragma once

#include "Component.h"
#include <string>
#include <iostream>

class CMesh : public Component{
public:
    CMesh(std::string);
    ~CMesh();

    std::string GetMesh() { return m_mesh;};

private:
    std::string m_mesh;
};
