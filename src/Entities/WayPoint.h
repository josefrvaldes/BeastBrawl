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
    WayPoint(glm::vec3 _position, int type, int id);


    glm::vec3 position; //Posición del waypoint
    float radious = 30.0;
    int type;   //Tipo de waypoint - 0 = normal / 1 = PowerUp
    int id;
private:
};
