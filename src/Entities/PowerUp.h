#pragma once

#include "Entity.h"
#include <glm/vec3.hpp>
#include <Components/CPowerUp.h>
#include <Components/CTransformable.h>
#include <iostream>
#include <map>

using namespace std;


class PowerUp : public Entity{

public:
    PowerUp();
    //PowerUp(glm::vec3 _position);
    PowerUp(glm::vec3 _position, glm::vec3 _rotation, typeCPowerUp typePowerUp, CTransformable* cTransformableTarget);
    ~PowerUp();


private:
    const string VERTEX_SHADER = "CLEngine/src/Shaders/vertex.glsl";
    const string FRAGMENT_SHADER = "CLEngine/src/Shaders/fragment.glsl";
};
