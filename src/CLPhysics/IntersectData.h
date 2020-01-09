#pragma once
#include "../../lib/glm/geometric.hpp"
#include "../../lib/glm/vec3.hpp"

using namespace glm;

class IntersectData {
   public:
    IntersectData(bool _intersects, vec3 direction_)
        : intersects{_intersects}, direction{direction_} {};
    IntersectData(bool _intersects, vec3 targetPosition_, float _distance)
        : intersects{_intersects}, direction{targetPosition_}, distance{_distance} {};

    float GetDistance() {
        return glm::length(direction);
    }
    const bool intersects;
    vec3 direction;
    float distance = 9999999;
};