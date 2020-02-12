#pragma once

#include "Component.h"

#include <iostream>

using namespace std;

class CCurrentNavMesh : public Component
{
public:
    CCurrentNavMesh(int r_currentNavMesh);
    ~CCurrentNavMesh(){};

//private:
    int currentNavMesh;  //NavMesh actual del coche/entidad 
};
