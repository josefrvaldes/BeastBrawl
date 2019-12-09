#include "../Entities/WayPoint.h"
#include "./ManWayPoint.h"


//Aqui va a leer directamente el archivo y crear los WayPoints
ManWayPoint::ManWayPoint(){
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

        cout << token << endl;
        glm::vec3 position = glm::vec3(x,y,z);
        CreateWayPoint(position,type,id);
        cont = 0;
    }

}


void ManWayPoint::CreateWayPoint(glm::vec3 _position) {
	shared_ptr<WayPoint> p = make_shared<WayPoint>(_position);
    waypoints.push_back(p);
    cout << "Creado waypoints, ahora tenemos " << waypoints.size() << " waypoints" << endl;
}

void ManWayPoint::CreateWayPoint(glm::vec3 _position,int type, int id) {
	shared_ptr<WayPoint> p = make_shared<WayPoint>(_position,type,id);
    waypoints.push_back(p);
    cout << "Creado waypoints, ahora tenemos " << waypoints.size() << " waypoints" << endl;
}

void ManWayPoint::CreateWayPoint() {
	shared_ptr<WayPoint> p = make_shared<WayPoint>();
    waypoints.push_back(p);
    cout << "Creado waypoints, ahora tenemos " << waypoints.size() << " waypoints" << endl;
}

ManWayPoint::~ManWayPoint(){
    waypoints.clear();
    waypoints.shrink_to_fit();
}
