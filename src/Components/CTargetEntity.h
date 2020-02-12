#pragma once

#include "Component.h"
#include "CTransformable.h"

#include <glm/vec3.hpp>
#include <string>
#include <iostream>

using namespace std;
//using namespace glm;

class CTargetEntity : public Component{
public:
    CTargetEntity();
    ~CTargetEntity();


    CTransformable* cTransTarget = nullptr;
    
};