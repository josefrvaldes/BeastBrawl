#include "WayPoint.h"
#include <Components/CId.h>
#include <Components/CWayPoint.h>
#include <Components/CWayPointEdges.h>
#include <iostream>


WayPoint::WayPoint()
{
    
}

//TODO: Este constructor hay que quitarlo 
WayPoint::WayPoint(glm::vec3 _position) {
    shared_ptr<CWayPoint> cWayPoint = make_shared<CWayPoint>(_position,0,0);
    shared_ptr<CWayPointEdges> cWayPointEdges = make_shared<CWayPointEdges>();
    AddComponent(cWayPoint);
    AddComponent(cWayPointEdges);
}

WayPoint::WayPoint(glm::vec3 _position, int _type, int _id){
    shared_ptr<CWayPoint> cWayPoint = make_shared<CWayPoint>(_position,_type,_id);
    AddComponent(cWayPoint);
}

void WayPoint::AddEdge(int to, float cost){
    //Tenemos que comprobar si ya tenia un WayPointEdges creado para no pisarlo
    if(!hasEdge){
        shared_ptr<CWayPointEdges> cWayPointEdges = make_shared<CWayPointEdges>(to,cost);
        AddComponent(cWayPointEdges);
        hasEdge = true;
    }else{
        auto cWayPointEdges = static_cast<CWayPointEdges*>(GetComponent(CompType::WayPointEdgesComp).get());
        cWayPointEdges->AddEdge(to,cost);
    }

}

WayPoint::~WayPoint(){

}
