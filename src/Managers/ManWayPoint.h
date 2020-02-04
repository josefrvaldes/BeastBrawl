#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <memory>
#include <fstream>
#include <string>
#include <cstdlib>
#include "../../include/glm/vec3.hpp"
#include "Manager.h"

class WayPoint;
using namespace std;

class ManWayPoint : public Manager{
public:
    ManWayPoint(); //Aqui leemos el archivo
    ~ManWayPoint();

    void CreateWayPoint(glm::vec3 _position);
    void CreateWayPoint(glm::vec3 _position, int type, int id);
    void CreateWayPoint();
    //vector<shared_ptr<WayPoint>> GetEntities() const {
    //    return waypoints;
    //};


private:
	//vector<shared_ptr<WayPoint>> waypoints;
    //void SubscribeToEvents();
};