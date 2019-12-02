#ifndef __CAR_H__
#define __CAR_H__

#include "Entity.h"
#include "../../lib/glm/vec3.hpp"
#include <iostream>
#include <string>
using namespace std;

class Car : public Entity
{

public:
    Car();
    Car(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, string texture, string mesh, float maxSpeed, float acceleration, float carFriction, float carSlowDown);
    ~Car();



};

#endif