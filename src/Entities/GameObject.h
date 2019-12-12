#ifndef __GameObject_H__
#define __GameObject_H__

#include "Entity.h"
#include "../../lib/glm/vec3.hpp"
#include <iostream>
#include <string>
using namespace std;

class GameObject : public Entity
{

public:
    GameObject();
    GameObject(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, string texture, string mesh);
    ~GameObject();
};

#endif