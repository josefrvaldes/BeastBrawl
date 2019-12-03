#include "PowerUp.h"
#include "../Components/CPosition.h"
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

PowerUp::PowerUp()
{
    // default values
    string texture = "earth.jpg";
    string mesh    = "media/ninja.b3d";
    float maxSpeed = 20.0, acceleration = .15, friction = 0.1, slowDown = 0.25;
    
    CId* cId   = new CId();
    CType* cType = new CType(ModelType::Cube);
    CTransformable* cTransformable = new CTransformable(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.0f,1.0f,1.0f)); 
    CTexture* cTexture = new CTexture(texture);
    CMesh* cMesh   = new CMesh(mesh);
    CCar* cCar = new CCar(maxSpeed, acceleration, friction, slowDown);
    AddComponent(cId);
    AddComponent(cType);
    AddComponent(cTransformable);
    AddComponent(cTexture);
    AddComponent(cMesh);
    AddComponent(cCar);
    cout << "Acabamos de llamar al constructor default de powerup, su transformable es " << cTransformable << endl;
}

PowerUp::PowerUp(float posX, float posY, float posZ) 
    : PowerUp()
{
    //CTransformable *cTransformable = (CTransformable *)m_components[CompType::TransformableComp];
    //cTransformable->posX = posX;
    //cTransformable->posY = posY;
    //cTransformable->posZ = posZ;
}



PowerUp::~PowerUp()
{

}
