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

#include "Component.h"

#include <string>
#include <iostream>
#include <glm/glm.hpp>

class CLight : public Component{
public:
    CLight();
    CLight(glm::vec3 in, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float cons, float lin, float quadr);
    ~CLight(){};

    glm::vec3 intensity {1.0f,1.0f,1.0f};
    glm::vec3 ambient   {0.9f,0.9f,0.9f};
    glm::vec3 diffuse   {0.2f,0.3f,0.42f};
    glm::vec3 specular  {0.1f,0.5f,0.6f};
    float constant      {1.0f};
    // float linear        {0.0000036f};
    // float quadratic     {0.00000036f};

    float linear        {0.0000025f};  // 30
    float quadratic     {0.00000010f};  // 28
    
};
