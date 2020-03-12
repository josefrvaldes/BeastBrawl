#include "ManLight.h"

#include <Entities/Light.h>

//TODO: Leerlo de fichero
ManLight::ManLight(){
    shared_ptr<Light> p = make_shared<Light>(glm::vec3(0.0,100.0,0.0));
    entities.push_back(p);

    shared_ptr<Light> p2 = make_shared<Light>(glm::vec3(100.0,100.0,0.0));
    entities.push_back(p2);

    shared_ptr<Light> p3 = make_shared<Light>(glm::vec3(0.0,100.0,100.0));
    entities.push_back(p3);

    shared_ptr<Light> p4 = make_shared<Light>(glm::vec3(100.0,100.0,100.0));
    entities.push_back(p4);

    shared_ptr<Light> p5 = make_shared<Light>(glm::vec3(-200.0,100.0,-100.0));
    entities.push_back(p5);

    shared_ptr<Light> p6 = make_shared<Light>(glm::vec3(300.0,100.0,-300.0));
    entities.push_back(p6);


}

void ManLight::CreateLight(glm::vec3 pos){
    shared_ptr<Light> p = make_shared<Light>(pos);
    entities.push_back(p);
}


void ManLight::CreateLight(glm::vec3 pos, glm::vec3 intensity, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic){
    shared_ptr<Light> p = make_shared<Light>(pos, intensity, ambient, diffuse, specular, constant, linear, quadratic);
    entities.push_back(p);
}