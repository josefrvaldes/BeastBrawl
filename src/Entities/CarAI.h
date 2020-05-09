#pragma once


#include "Car.h"
#include <string>
#include <stack>
#include <iostream>
#include <glm/vec3.hpp>
#include <Components/CPosDestination.h>
#include <Components/CWayPoint.h>

struct CWayPoint;
struct CPosDestination;

class CarAI : public Car{

   public:
    CarAI(int pj);
    CarAI(int pj, glm::vec3 pos);
    CarAI(int pj, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, string texture, string mesh, float maxSpeed, float acceleration, float carFriction, float carSlowDown, std::string vertexShader, std::string fragmentShader);
    ~CarAI();



    void SetWayPoint(CWayPoint* _waypoint);
    void SetDestination(CPosDestination* posDestination);
    void SetPath(stack<int> path);
    void SetRotation(glm::vec3 rot_);

};

