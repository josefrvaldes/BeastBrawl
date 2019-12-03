#pragma once

#include "../../lib/glm/vec3.hpp"

#include "Component.h"
#include <string>
#include <iostream>

using namespace std;
//using namespace glm;

class CLastPosition : public Component
{
public:
    CLastPosition();
    CLastPosition(glm::vec3 _pos);
    ~CLastPosition();

    friend ostream &operator<<(ostream &out, CLastPosition &c)
    {
        out << "(" << c.x << "," << c.y << "," << c.z << ")";
        return out;
    }

    glm::vec3 position;
};
