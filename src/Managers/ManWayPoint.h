#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <memory>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "../../include/glm/vec3.hpp"

class WayPoint;
using namespace std;

class ManWayPoint{
public:
    ManWayPoint(); //Aqui leemos el archivo
    ~ManWayPoint();

    void CreateWayPoint(glm::vec3 _position);
    void CreateWayPoint(glm::vec3 _position, int type, int id);
    void CreateWayPoint();
    vector<shared_ptr<WayPoint>> GetEntities() const {
        return waypoints;
    };


private:
	vector<shared_ptr<WayPoint>> waypoints;
    //void SubscribeToEvents();
};