#pragma once
#include "../../include/glm/geometric.hpp"
#include "../../include/glm/vec3.hpp"

using namespace glm;

class IntersectData {
   public:
    IntersectData(bool _intersects, vec3 direction_)
        : intersects{_intersects}, direction{direction_} {};

    float GetDistance() {
        return glm::length(direction);
    }
    const bool intersects;
    vec3 direction;
};