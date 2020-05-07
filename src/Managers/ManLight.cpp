#include "ManLight.h"

#include <Entities/Light.h>

//TODO: Leerlo de fichero
ManLight::ManLight(){
    shared_ptr<Light> p = make_shared<Light>(glm::vec3(-50.0,400.0,100.0));
    entities.push_back(p);
}

void ManLight::CreateLight(glm::vec3 pos){
    shared_ptr<Light> p = make_shared<Light>(pos);
    entities.push_back(p);
}


void ManLight::CreateLight(glm::vec3 pos, glm::vec3 intensity, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic){
    shared_ptr<Light> p = make_shared<Light>(pos, intensity, ambient, diffuse, specular, constant, linear, quadratic);
    entities.push_back(p);
}