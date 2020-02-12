#pragma once

#include "Component.h"

#include <glm/vec3.hpp>
#include <string>
#include <iostream>

using namespace std;
//using namespace glm;

class CWayPoint : public Component
{
public:
    CWayPoint();
    CWayPoint(glm::vec3 _pos, int _type,int _id);
    ~CWayPoint();

    glm::vec3 position;
    int type;
    int id;
    float radious;
};