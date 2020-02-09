#pragma once

#include "Entity.h"
#include <iostream>
#include <map>
#include <memory>

#include <glm/vec3.hpp>

using namespace std;

class WayPoint : public Entity{
public:
    WayPoint();
    ~WayPoint();
    WayPoint(glm::vec3 _position);
    WayPoint(glm::vec3 _position, int type, int id);
    void AddEdge(int to, float cost);

private:
    bool hasEdge = false;

};
