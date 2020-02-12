#pragma once

#include "Component.h"

#include <iostream>

using namespace std;

class CTargetNavMesh : public Component
{
public:
    CTargetNavMesh(int r_currentNavMesh);
    ~CTargetNavMesh(){};

//private:
    int targetNavMesh;  //NavMesh actual del coche/entidad 
};
