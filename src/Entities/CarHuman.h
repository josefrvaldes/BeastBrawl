#pragma once

#include "Car.h"

#include <string>
#include <glm/vec3.hpp>

class CarHuman : public Car {
   public:
    CarHuman(int pj, int timeTotem);
    CarHuman(int pj, int timeTotem, glm::vec3 pos);
    CarHuman(int pj, int timeTotem, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, string texture, string mesh, float maxSpeed, float acceleration, float carFriction, float carSlowDown, string vertexShader, string fragmentShader);
    ~CarHuman();
    
};
