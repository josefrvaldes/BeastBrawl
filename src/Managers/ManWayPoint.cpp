#include "../Entities/WayPoint.h"
#include "./ManWayPoint.h"
#include "../Components/CWayPoint.h"
#include "../../include/include_json/include_json.hpp"

#include <cmath>

using json = nlohmann::json;
//Aqui va a leer directamente el archivo y crear los WayPoints
ManWayPoint::ManWayPoint(){

    //Leemos y añadimos los WayPoints
    float x=0,y=0,z=0; //Vec3
    float cost = 0;
    int id=0, type=0,to=0,from=0;


    ifstream i("WayPoints.json");
    json j = json::parse(i);

    int waypointsCount = j["WayPoints"].size();

    //Leemos el array de waypoints
    for(int i = 0; i< waypointsCount; ++i){
        auto capaActual = j["WayPoints"][i];
        id   = capaActual["ID"].get<int>();
        x    = capaActual["POS_X"].get<float>();
        y    = capaActual["POS_Y"].get<float>();
        z    = capaActual["POS_Z"].get<float>();
        type = capaActual["TYPE"].get<int>();

        cout << id << "/" << x << "/" << y << "/" << z << "/" << type << "/" << endl;
        glm::vec3 position = glm::vec3(x,y,z);
        CreateWayPoint(position,type,id);
    }

    int edgesCount = j["Edges"].size();

    //Leemos el array de edges
    for(int i = 0; i< edgesCount; ++i){
        auto capaActual = j["Edges"][i];
        from  = capaActual["FROM"].get<int>();
        to    = capaActual["TO"].get<int>();

        //TODO: Calcular el coste
        auto componentsFrom = waypoints[from]->GetComponents();
	    auto cTransformableFrom = static_cast<CWayPoint*>(componentsFrom[CompType::WayPointComp].get());

        auto componentsTo = waypoints[to]->GetComponents();
	    auto cTransformableTo = static_cast<CWayPoint*>(componentsTo[CompType::WayPointComp].get());

        float x = cTransformableFrom->position.x - cTransformableTo->position.x;
        float y = cTransformableFrom->position.y - cTransformableTo->position.y;
        float z = cTransformableFrom->position.z - cTransformableTo->position.z;

        cost = std::sqrt(x*x + y*y + z*z);

        //Añadimos tanto a uno como a otro porque es un grafo no dirigido
        waypoints[from]->AddEdge(to,cost); 
        waypoints[to]->AddEdge(from,cost);

    }


}


void ManWayPoint::CreateWayPoint(glm::vec3 _position) {
	shared_ptr<WayPoint> p = make_shared<WayPoint>(_position);
    waypoints.push_back(p);
}

void ManWayPoint::CreateWayPoint(glm::vec3 _position,int type, int id) {
	shared_ptr<WayPoint> p = make_shared<WayPoint>(_position,type,id);
    waypoints.push_back(p);
}

void ManWayPoint::CreateWayPoint() {
	shared_ptr<WayPoint> p = make_shared<WayPoint>();
    waypoints.push_back(p);
}

ManWayPoint::~ManWayPoint(){
    waypoints.clear();
    waypoints.shrink_to_fit();
}
