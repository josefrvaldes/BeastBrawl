#pragma once

#include "Entity.h"
#include <glm/vec3.hpp>
#include <Components/CPowerUp.h>
#include <Managers/Manager.h>
#include <iostream>
#include <map>

using namespace std;


class Shield : public Entity{

public:
    Shield();
    Shield(uint16_t parentId, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

    ~Shield();

};
