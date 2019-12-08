#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <memory>
#include "../../lib/glm/vec3.hpp"

class WayPoint;
class WayPointEdge;
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

    vector<shared_ptr<WayPointEdge>> GetEntitiesEdges() const {
        return waypointsEdges;
    };

private:
	vector<shared_ptr<WayPoint>> waypoints;
	vector<shared_ptr<WayPointEdge>> waypointsEdges;
    //void SubscribeToEvents();
};