#pragma once

#include "Component.h"

#include <iostream>
#include <string>
#include <vector>

class CMesh : public Component {
   public:
    CMesh() = delete;
    CMesh(std::string path);
    CMesh(std::string pathCerca, std::string pathMedio, std::string pathLejos);
    ~CMesh(){};

    std::vector<std::string> mesh;
    std::string activeMesh;

    //float distanceNear{250};
    //float distanceMedium{400};

    float distanceNear{5000};
    float distanceMedium{10000};
};
