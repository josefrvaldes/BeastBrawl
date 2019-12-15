#pragma once

class IntersectData {
   public:
    IntersectData(bool _intersects, float _distance) : intersects{_intersects}, distance{_distance} {

    };
    const bool intersects;
    const float distance;
};