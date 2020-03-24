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
    float constant      {0.05f};
    float linear        {0.00015};
    float quadratic     {0.000006};
    
};
