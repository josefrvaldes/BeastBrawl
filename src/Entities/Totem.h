#pragma once

#include "Entity.h"
#include <glm/vec3.hpp>
#include <Components/CPowerUp.h>
#include <Managers/Manager.h>
#include <iostream>
#include <map>

using namespace std;


class Totem : public Entity{

public:
    Totem(int time);
    //PowerUp(glm::vec3 _position);
    Totem(glm::vec3 _position, int time);
    Totem(glm::vec3 _position, glm::vec3 _rotation, int time);
    Totem(glm::vec3 _position, glm::vec3 _rotation, string texture, string mesh, int time);
    void Update(Manager* manNavMesh);
    ~Totem();

};
