#pragma once

#include "Component.h"

#include <string>
#include <iostream>
#include <vector>

class CMesh : public Component{
public:
    CMesh(std::string path);
    CMesh(std::string pathCerca, std::string pathMedio, std::string pathLejos);
    ~CMesh(){};


    std::vector<std::string> mesh;
};
