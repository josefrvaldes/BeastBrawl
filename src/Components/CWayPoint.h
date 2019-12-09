#pragma once

#include "../../lib/glm/vec3.hpp"

#include "Component.h"
#include <string>
#include <iostream>

using namespace std;
//using namespace glm;

class CWayPoint : public Component
{
public:
    CWayPoint();
    CWayPoint(glm::vec3 _pos, int type,int id);
    ~CWayPoint();

    // friend ostream &operator<<(ostream &out, CWayPoint &c)
    // {
    //     //cout << "(" << c.position.x << "," << c.position.y << "," << c.position.z << ")";
    //     //return out;
    // }

    glm::vec3 position;
    int m_type;
    int m_id;
    float radious;
    CWayPoint* padre;
    bool isFrontera;
    bool isInterior;
};
