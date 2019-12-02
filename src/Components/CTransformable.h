#pragma once

#include "Component.h"
#include "../../lib/glm/vec3.hpp"
#include <string>
#include <iostream>

using namespace std;

class CTransformable : public Component
{
public:
    CTransformable();
    CTransformable(glm::vec3 _pos, glm::vec3 _rot, glm::vec3 _scale);
    ~CTransformable();

    friend ostream &operator<<(ostream &out, CTransformable &c)
    {
        //out << "(" << c.GetX() << "," << c.GetY() << "," << c.GetZ() << ")";
        return out;
    }

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};