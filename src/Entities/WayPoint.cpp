#include "WayPoint.h"
//#include "../Components/CPosition.h"
#include "../Components/CSpeed.h"
#include "../Components/CId.h"
#include "../Components/CType.h"
#include "../Components/CTexture.h"
#include "../Components/CMesh.h"
#include "../Components/CTransformable.h"
#include "../Components/CCar.h"
#include <iostream>

class Position;
using namespace std;

WayPoint::WayPoint()
{
    // default values
    CTransformable* cTransformable = new CTransformable(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.0f,1.0f,1.0f)); 
    AddComponent(cTransformable);
    cout << "Acabamos de llamar al constructor default de powerup, su transformable es " << cTransformable << endl;
}

WayPoint::WayPoint(glm::vec3 _position) 
    : WayPoint(){
    CTransformable *cTransformable = (CTransformable *)m_components[CompType::TransformableComp];
    cTransformable->position.x = _position.x;
    cTransformable->position.y = _position.y;
    cTransformable->position.z = _position.z;

    posX = _position.x;
    posY = _position.y;
    posZ = _position.z;
}


WayPoint::~WayPoint(){

}
