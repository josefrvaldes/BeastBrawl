#pragma once

#include "../../lib/glm/vec3.hpp"

#include "Component.h"
#include <string>
#include <iostream>

using namespace std;
//using namespace glm;

class CPosDestination : public Component
{
public:
    CPosDestination();
    CPosDestination(glm::vec3 _pos, int _type,int _id);
    ~CPosDestination();

    glm::vec3 position;
    int id;
    float radious;
};