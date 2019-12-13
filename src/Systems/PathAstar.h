#pragma once

#include <vector>
#include <memory>

class WayPoint;
using namespace std;

class PathAstar{

public:
    PathAstar();
    ~PathAstar(){};

    void init();
    vector<shared_ptr<WayPoint>> update();

private:
	vector<shared_ptr<WayPoint>> listWaypoints;
	vector<shared_ptr<WayPoint>> listFrontera;
	vector<shared_ptr<WayPoint>> listInterior;
};