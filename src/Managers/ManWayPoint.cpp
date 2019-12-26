#include "../Entities/WayPoint.h"
#include "./ManWayPoint.h"
#include "../Components/CWayPoint.h"

#include <cmath>
//Aqui va a leer directamente el archivo y crear los WayPoints
ManWayPoint::ManWayPoint(){

    //Leemos y añadimos los WayPoints
    ifstream fe;
    fe.open("WayPoints.txt");
    string value; //Valor a leer
    string delimeter = "/"; //Valor para separar
    string token;
    size_t pos = 0;
    float x=0,y=0,z=0; //Vec3
    int cont = 0, id=0, type=0;


    while(!fe.eof()){
        fe >> value;
        while((pos = value.find(delimeter)) != string::npos){
            token = value.substr(0,pos);
            value.erase(0,pos+delimeter.length());

            if(cont==0){
                //ID
                id = ::atoi(token.c_str());
            }else if(cont==1){
                //vec3.x
                x = ::atof(token.c_str());
            }else if(cont==2){
                //vec3.y
                y = ::atof(token.c_str());
            }else if(cont==3){
                //vec3.z
                z = ::atof(token.c_str());
            }else if(cont==4){
                type = ::atoi(token.c_str());
            }

            cont++;
        }

        glm::vec3 position = glm::vec3(x,y,z);
        CreateWayPoint(position,type,id);
        cont = 0;
    }

    AddEdges();

}

void ManWayPoint::AddEdges(){
    //Leemos y añadimos los Edges
    ifstream fe;
    fe.open("Edges.txt");
    string value; //Valor a leer
    string delimeter = "/"; //Valor para separar
    string token;
    size_t pos = 0;
    float cost = 0.0;
    int from = 0, to = 0;
    int cont = 0;


    while(!fe.eof()){
        fe >> value;
        while((pos = value.find(delimeter)) != string::npos){
            token = value.substr(0,pos);
            value.erase(0,pos+delimeter.length());

            if(cont==0){
                //From
                from = ::atoi(token.c_str());
            }else if(cont==1){
                //To
                to = ::atoi(token.c_str());
            }

            cont++;
        }
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
        cont = 0;
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
