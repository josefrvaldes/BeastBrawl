/**
 * Beast Brawl
 * Game created as a final project of the Multimedia Engineering Degree in the University of Alicante.
 * Made by Clover Games Studio, with members 
 * Carlos de la Fuente Torres delafuentetorresc@gmail.com,
 * Antonio Jose Martinez Garcia https://www.linkedin.com/in/antonio-jose-martinez-garcia/,
 * Jesús Mas Carretero jmasc03@gmail.com, 
 * Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/, 
 * Rubén Rubio Martínez https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/, 
 * and Jose Valdés Sirvent https://www.linkedin.com/in/jose-f-valdés-sirvent-6058b5a5/ github -> josefrvaldes
 * 
 * 
 * @author Clover Games Studio
 * 
 */
 
 
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
