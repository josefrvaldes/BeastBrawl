#pragma once

#include <iostream>
#include <map>
#include <vector>
#include "../../lib/glm/vec3.hpp"

class WayPoint;
using namespace std;

class ManWayPoint{
public:
    ManWayPoint();
    ~ManWayPoint();

    void CreateWayPoint(glm::vec3 _position);
    void CreateWayPoint();
    vector<WayPoint *> GetEntities() const {
        return waypoints;
    };

private:
	vector<WayPoint *> waypoints;
    //void SubscribeToEvents();
};