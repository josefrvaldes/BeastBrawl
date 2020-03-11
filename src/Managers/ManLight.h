#pragma once

#include <Managers/Manager.h>
#include <iostream>
#include <memory>
#include <string>
#include <glm/vec3.hpp>

using namespace std;

class ManLight : public Manager{
public:
    ManLight(){}; 
    ~ManLight(){};

    void CreateLight(glm::vec3 pos);
    void CreateLight(glm::vec3 pos, glm::vec3 intensity, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic);

private:

};