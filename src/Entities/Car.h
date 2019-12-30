#pragma once

#include <iostream>
#include <string>
#include "../../lib/glm/vec3.hpp"
#include "Entity.h"
using namespace std;

class Car : public Entity {
   public:
    Car();
    Car(glm::vec3 pos);
    Car(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, string texture, string mesh, float maxSpeed, float acceleration, float carFriction, float carSlowDown);
    ~Car();
    
};
