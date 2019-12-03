#include "../Entities/WayPoint.h"
#include "./ManWayPoint.h"
#include <iostream>

class Position;
using namespace std;

ManWayPoint::ManWayPoint(){
    //SubscribeToEvents();
    //CreateWayPoint(30, 30, 30);
    cout << "Hemos creado el array de waypoints, ahora tenemos " << waypoints.size() << " waypoints" << endl;
}

ManWayPoint::~ManWayPoint(){
    waypoints.clear();
    waypoints.shrink_to_fit();
}

void ManWayPoint::CreateWayPoint(float x, float y, float z) {
	WayPoint *p = new WayPoint(x, y, z);
    waypoints.push_back(p);
    cout << "Creado waypoints, ahora tenemos " << waypoints.size() << " waypoints" << endl;
}

void ManWayPoint::CreateWayPoint() {
	WayPoint *p = new WayPoint();
    waypoints.push_back(p);
    cout << "Creado waypoints, ahora tenemos " << waypoints.size() << " waypoints" << endl;
}

