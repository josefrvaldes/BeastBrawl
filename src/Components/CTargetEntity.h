#pragma once

#include "../../lib/glm/vec3.hpp"
#include "../Components/CTransformable.h"

#include "Component.h"
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