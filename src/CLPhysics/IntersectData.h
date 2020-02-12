#pragma once
#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

using namespace glm;

class IntersectData {
   public:
    IntersectData(bool _intersects, vec3 direction_)
        : intersects{_intersects}, direction{direction_} {};
    IntersectData(bool _intersects, vec3 targetPosition_, float _distance)
        : intersects{_intersects}, direction{targetPosition_}, distance{_distance} {};

    // con la especificacion de elemento de un conjunto (como OBB que contiene varios planos)
    IntersectData(bool _intersects, int posEntity_, vec3 direction_)
        : intersects{_intersects}, direction{direction_}, posEntity{posEntity_} {};
    IntersectData(bool _intersects, int posEntity_, vec3 targetPosition_, float _distance)
        : intersects{_intersects}, direction{targetPosition_}, distance{_distance}, posEntity{posEntity_} {};

    // con los valores de "centro" y "radio" de la esfera virtual del cilindro
    IntersectData(bool intersects_, vec3 direction_, vec3 virtualCenter_, float virtualRadius_ )
        : intersects{intersects_}, direction{direction_}, virtualCenter{virtualCenter_}, virtualRadius{virtualRadius_} {};

    float GetDistance() {
        return glm::length(direction);
    }
    const bool intersects;
    vec3 direction;
    float distance = 9999999;
    int posEntity; // cuando pasamos un OBB, por ejemplo, y colisionamos con uno de sus planos, para saber con que plano colisionamos

    vec3 virtualCenter;
    float virtualRadius;
};