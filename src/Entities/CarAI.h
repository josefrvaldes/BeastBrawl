#pragma once

#include "Entity.h"
#include "../../include/glm/vec3.hpp"
#include <iostream>
#include <string>
#include <stack>
#include "../Components/CWayPoint.h"
#include "../Components/CPosDestination.h"
using namespace std;

class CarAI : public Entity
{

public:
    CarAI();
    CarAI(glm::vec3 pos);
    CarAI(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, string texture, string mesh, float maxSpeed, float acceleration, float carFriction, float carSlowDown);
    ~CarAI();



    void SetWayPoint(CWayPoint* _waypoint);
    void SetDestination(CPosDestination* posDestination);
    void SetPath(stack<int> path);

};

