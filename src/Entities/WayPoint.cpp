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
    float posX = 0.0,  posY = 0.0,  posZ = 0.0;
    float rotX = 0.0,  rotY = 0.0,  rotZ = 0.0;
    float scaX = 1.0,  scaY = 1.0,  scaZ = 1.0;
    CTransformable* cTransformable = new CTransformable(posX, posY, posZ, rotX, rotY, rotZ, scaX, scaY, scaZ); 
    AddComponent(cTransformable);
    cout << "Acabamos de llamar al constructor default de powerup, su transformable es " << cTransformable << endl;
}

WayPoint::WayPoint(float pX, float pY, float pZ) 
    : WayPoint(){
    CTransformable *cTransformable = (CTransformable *)m_components[CompType::TransformableComp];
    cTransformable->posX = pX;
    cTransformable->posY = pY;
    cTransformable->posZ = pZ;

    posX = pX;
    posY = pY;
    posZ = pZ;
}


WayPoint::~WayPoint(){

}
