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
