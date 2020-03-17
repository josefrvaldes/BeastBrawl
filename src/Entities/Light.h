#pragma once

#include "Entity.h"
#include <glm/vec3.hpp>
#include <iostream>
#include <string>
using namespace std;

class Light : public Entity
{

public:
    Light(glm::vec3 pos);
    Light(glm::vec3 pos, glm::vec3 intensity, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic);
    ~Light();
};
