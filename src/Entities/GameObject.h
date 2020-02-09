#pragma once

#include "Entity.h"
#include <glm/vec3.hpp>
#include <iostream>
#include <string>
using namespace std;

class GameObject : public Entity
{

public:
    GameObject();
    GameObject(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, const string& texture, const string& mesh);
    ~GameObject();
};
