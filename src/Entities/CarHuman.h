#pragma once

#include "Car.h"

#include <string>
#include <glm/vec3.hpp>

class CarHuman : public Car {
   public:
    CarHuman(int pj);
    CarHuman(int pj, glm::vec3 pos);
    ~CarHuman();
    
};
