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
 
 
 #include "ManWayPoint.h"

#include <Entities/WayPoint.h>
#include <Components/CWayPoint.h>
#include <include_json/include_json.hpp>

#include <cmath>

using json = nlohmann::json;
//Aqui va a leer directamente el archivo y crear los WayPoints
ManWayPoint::ManWayPoint(){

    //Leemos y añadimos los WayPoints
    float x=0,y=0,z=0; //Vec3
    float cost = 0;
    int id=0, type=0,to=0,from=0;

    ifstream i("data.json");
    json j = json::parse(i);

    int waypointsCount = j["WAYPOINTS"].size();
    //std::cout << "EL NUMERO DE WAYPOINTS EN EL JSON ES: " << waypointsCount << std::endl;
    //Leemos el array de waypoints
    for(int i = 0; i< waypointsCount; ++i){
        string idWayPoint = to_string(i);  //El primer elemento es "0" como string
        auto capaActual = j["WAYPOINTS"][idWayPoint];
        id   = capaActual["id"].get<int>();
        x    = capaActual["x"].get<float>();
        y    = capaActual["y"].get<float>();    // INTERCAMBIAMOS REALMENTE LA "Y" POR LA "Z"
        z    = capaActual["z"].get<float>();
        type = capaActual["type"].get<int>();

        //cout << id << "/" << x << "/" << y << "/" << z << "/" << type << "/" << endl;
        glm::vec3 position = glm::vec3(x,y,z);
        CreateWayPoint(position,type,id);
    }

    int edgesCount = j["EDGES"].size();
    //std::cout << "EL NUMERO DE EDGES EN EL JSON ES: " << edgesCount << std::endl;
    //Leemos el array de edges
    for(int i = 0; i< edgesCount; ++i){
        string idEdge = to_string(i);  //El primer elemento es "0" como string
        auto edgeActual = j["EDGES"][idEdge];
        from  = edgeActual["from"].get<int>();
        to    = edgeActual["to"].get<int>();

        //TODO: Calcular el coste
        auto componentsFrom = entities[from]->GetComponents();
	    auto cTransformableFrom = static_cast<CWayPoint*>(componentsFrom[CompType::WayPointComp].get());

        auto componentsTo = entities[to]->GetComponents();
	    auto cTransformableTo = static_cast<CWayPoint*>(componentsTo[CompType::WayPointComp].get());

        float x = cTransformableFrom->position.x - cTransformableTo->position.x;
        float y = cTransformableFrom->position.y - cTransformableTo->position.y;
        float z = cTransformableFrom->position.z - cTransformableTo->position.z;

        cost = std::sqrt(x*x + y*y + z*z);
        //std::cout << "El coste de este edge es: " << cost << std::endl;

        //Añadimos tanto a uno como a otro porque es un grafo no dirigido
        static_cast<WayPoint*>(entities[from].get())->AddEdge(to,cost); 
        static_cast<WayPoint*>(entities[to].get())->AddEdge(from,cost);

    }

/*
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
    //Lemos el array de edges
    for(int i = 0; i< edgesCount; ++i){
        auto capaActual = j["Edges"][i];
        from  = capaActual["FROM"].get<int>();
        to    = capaActual["TO"].get<int>();
        //TODO: Calcular el coste
        auto componentsFrom = entities[from]->GetComponents();
	    auto cTransformableFrom = static_cast<CWayPoint*>(componentsFrom[CompType::WayPointComp].get());
        auto componentsTo = entities[to]->GetComponents();
	    auto cTransformableTo = static_cast<CWayPoint*>(componentsTo[CompType::WayPointComp].get());
        float x = cTransformableFrom->position.x - cTransformableTo->position.x;
        float y = cTransformableFrom->position.y - cTransformableTo->position.y;
        float z = cTransformableFrom->position.z - cTransformableTo->position.z;
        cost = std::sqrt(x*x + y*y + z*z);
        //Añadimos tanto a uno como a otro porque es un grafo no dirigido
        static_cast<WayPoint*>(entities[from].get())->AddEdge(to,cost); 
        static_cast<WayPoint*>(entities[to].get())->AddEdge(from,cost);
    }
*/
}


void ManWayPoint::CreateWayPoint(glm::vec3 _position) {
	shared_ptr<WayPoint> p = make_shared<WayPoint>(_position);
    entities.push_back(p);
}

void ManWayPoint::CreateWayPoint(glm::vec3 _position,int type, int id) {
	shared_ptr<WayPoint> p = make_shared<WayPoint>(_position,type,id);
    entities.push_back(p);
}

void ManWayPoint::CreateWayPoint() {
	shared_ptr<WayPoint> p = make_shared<WayPoint>();
    entities.push_back(p);
}

ManWayPoint::~ManWayPoint(){
    entities.clear();
    entities.shrink_to_fit();
}
