#pragma once

#include "Entity.h"

#include <glm/vec3.hpp>
#include <iostream>
#include <map>


using namespace std;


class BoxPowerUp : public Entity{

public:

    BoxPowerUp();
    BoxPowerUp(glm::vec3 _position);
    ~BoxPowerUp();
};
