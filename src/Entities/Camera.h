#pragma once

#include "Entity.h"

#include <EventManager/EventManager.h>
#include <EventManager/Event.h>

#include <glm/vec3.hpp>
#include <iostream>

using namespace std;

class Camera : public Entity
{

public:
    Camera();
    Camera(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);
    Camera(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, float maxSpeed_, float acc_, float revMaxSpeed_, float slowDown);
    ~Camera();

    void SuscribeEvents();

    void InvertCamera(DataMap* d);
    void TotemCamera(DataMap* d);
    void NormalCamera(DataMap* d);
};


