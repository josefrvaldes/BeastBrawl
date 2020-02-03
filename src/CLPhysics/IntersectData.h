#pragma once
#include "../../include/glm/geometric.hpp"
#include "../../include/glm/vec3.hpp"

using namespace glm;

class IntersectData {
   public:
    IntersectData(bool _intersects, vec3 direction_)
        : intersects{_intersects}, direction{direction_} {};
    IntersectData(bool _intersects, vec3 targetPosition_, float _distance)
        : intersects{_intersects}, direction{targetPosition_}, distance{_distance} {};
    IntersectData(bool _intersects, int posEntity_, vec3 direction_)
        : intersects{_intersects}, direction{direction_}, posEntity{posEntity_} {};

    float GetDistance() {
        return glm::length(direction);
    }
    const bool intersects;
    vec3 direction;
    float distance = 9999999;
    int posEntity; // cuando pasamos un OBB, por ejemplo, y colisionamos con uno de sus planos, para saber con que plano colisionamos
};