#pragma once

#include "Component.h"

#include <iostream>
#include <string>
#include <glm/vec3.hpp>

using namespace std;
using namespace glm;

class CSpeed : public Component {
   public:
    CSpeed();
    CSpeed(vec3 _speed);
    ~CSpeed(){};

    friend ostream &operator<<(ostream &out, CSpeed &c) {
        out << "(" << c.speed.x << "," << c.speed.y << "," << c.speed.z << ")";
        return out;
    }

    vec3 speed;
};
