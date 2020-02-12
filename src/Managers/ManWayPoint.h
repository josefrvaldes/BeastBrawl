#pragma once

#include "Manager.h"

#include <iostream>
#include <map>
#include <vector>
#include <memory>
#include <fstream>
#include <string>
#include <cstdlib>
#include <glm/vec3.hpp>

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