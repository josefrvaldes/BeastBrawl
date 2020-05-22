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