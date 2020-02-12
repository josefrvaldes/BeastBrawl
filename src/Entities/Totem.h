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
    Totem();
    //PowerUp(glm::vec3 _position);
    Totem(glm::vec3 _position);
    Totem(glm::vec3 _position, glm::vec3 _rotation);
    Totem(glm::vec3 _position, glm::vec3 _rotation, string texture, string mesh);
    void Update(Manager* manNavMesh);
    ~Totem();

};
