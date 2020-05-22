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
#include "../../include/glm/geometric.hpp"
#include "../../include/glm/vec3.hpp"

using namespace glm;

class IntersectData {
   public:
    IntersectData(bool _intersects, glm::vec3 direction_)
        : intersects{_intersects}, direction{direction_} {};
    IntersectData(bool _intersects, glm::vec3 targetPosition_, float _distance)
        : intersects{_intersects}, direction{targetPosition_}, distance{_distance} {};

    // con la especificacion de elemento de un conjunto (como OBB que contiene varios planos)
    IntersectData(bool _intersects, int posEntity_, glm::vec3 direction_)
        : intersects{_intersects}, direction{direction_}, posEntity{posEntity_} {};
    IntersectData(bool _intersects, int posEntity_, glm::vec3 targetPosition_, float _distance)
        : intersects{_intersects}, direction{targetPosition_}, distance{_distance}, posEntity{posEntity_} {};

    // con los valores de "centro" y "radio" de la esfera virtual del cilindro
    IntersectData(bool intersects_, glm::vec3 direction_, glm::vec3 virtualCenter_, float virtualRadius_ )
        : intersects{intersects_}, direction{direction_}, virtualCenter{virtualCenter_}, virtualRadius{virtualRadius_} {};

    float GetDistance() {
        return glm::length(direction);
    }
    //const bool intersects;
    bool intersects;
    glm::vec3 direction;
    float distance = 9999999;
    int posEntity; // cuando pasamos un OBB, por ejemplo, y colisionamos con uno de sus planos, para saber con que plano colisionamos

    glm::vec3 virtualCenter;
    float virtualRadius;
};