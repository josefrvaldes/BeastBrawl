#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <memory>
#include "../../lib/glm/vec3.hpp"

class WayPoint;
using namespace std;

class ManWayPoint{
public:
    ManWayPoint();
    ~ManWayPoint();

    void CreateWayPoint(glm::vec3 _position);
    void CreateWayPoint();
    vector<shared_ptr<WayPoint>> GetEntities() const {
        return waypoints;
    };

private:
	vector<shared_ptr<WayPoint>> waypoints;
    //void SubscribeToEvents();
};