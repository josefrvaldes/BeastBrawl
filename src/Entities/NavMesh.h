#pragma once

#include "Entity.h"
#include <glm/vec3.hpp>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class NavMesh : public Entity
{

public:
    NavMesh(glm::vec3 pos, glm::vec3 rot, float width, float height, float depth, const vector<int>& waypoints);
    ~NavMesh();

};

