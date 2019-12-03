#pragma once

#include <iostream>
#include <map>
#include <vector>

class WayPoint;
using namespace std;

class ManWayPoint{
public:
    ManWayPoint();
    ~ManWayPoint();

    void CreateWayPoint(float x, float y, float z);
    void CreateWayPoint();
    vector<WayPoint *> GetEntities() const {
        return waypoints;
    };

private:
	vector<WayPoint *> waypoints;
    //void SubscribeToEvents();
};