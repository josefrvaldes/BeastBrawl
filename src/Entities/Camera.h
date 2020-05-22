/**
 * Beast Brawl
 * Game created as a final project of the Multimedia Engineering Degree in the University of Alicante.
 * Made by Clover Games Studio, with members 
 * Carlos de la Fuente Torres delafuentetorresc@gmail.com,
 * Antonio Jose Martinez Garcia https://www.linkedin.com/in/antonio-jose-martinez-garcia/,
 * Jesús Mas Carretero jmasc03@gmail.com, 
 * Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/, 
 * Rubén Rubio Martínez https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/, 
 * and Jose Valdés Sirvent https://www.linkedin.com/in/jose-f-valdés-sirvent-6058b5a5/ github -> josefrvaldes
 * 
 * 
 * @author Clover Games Studio
 * 
 */
 
 
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
    void setTarget(glm::vec3);

    void InvertCamera(DataMap* d);
    void TotemCamera(DataMap* d);
    void NormalCamera(DataMap* d);
};


