#pragma once

#include <iostream>
#include <string>
#include "../../lib/glm/vec3.hpp"
#include "Component.h"

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
