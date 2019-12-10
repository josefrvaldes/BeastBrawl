#include "WayPoint.h"
#include "../Components/CSpeed.h"
#include "../Components/CId.h"
#include "../Components/CType.h"
#include "../Components/CTexture.h"
#include "../Components/CMesh.h"
#include "../Components/CTransformable.h"
#include "../Components/CCar.h"
#include "../Components/CWayPoint.h"
#include <iostream>

class Position;
using namespace std;

WayPoint::WayPoint()
{
    // default values
    shared_ptr<CTransformable> cTransformable = make_shared<CTransformable>(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.0f,1.0f,1.0f)); 
    AddComponent(cTransformable);
    cout << "Acabamos de llamar al constructor default de powerup, su transformable es " << cTransformable << endl;
}

WayPoint::WayPoint(glm::vec3 _position) {
    //position = _position;
}

WayPoint::WayPoint(glm::vec3 _position, int _type, int _id){
    shared_ptr<CWayPoint> cWayPoint = make_shared<CWayPoint>(_position,_type,_id);
    AddComponent(cWayPoint);
}


WayPoint::~WayPoint(){

}
