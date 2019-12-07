#pragma once

#include "Entity.h"
#include <iostream>
#include <map>
#include <memory>

#include "../../lib/glm/vec3.hpp"

using namespace std;

class WayPoint : public Entity{
public:
    WayPoint();
    ~WayPoint();
    WayPoint(glm::vec3 _position);


    glm::vec3 position;
    float radious = 5.0;
private:
};
