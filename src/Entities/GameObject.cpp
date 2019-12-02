#include "GameObject.h"
#include "../Components/CPosition.h"
#include "../Components/CSpeed.h"
#include "../Components/CId.h"
#include "../Components/CType.h"
#include "../Components/CTexture.h"
#include "../Components/CMesh.h"
#include "../Components/CTransformable.h"
#include "../Components/CCar.h"
#include "iostream"

class Position;
using namespace std;

GameObject::GameObject()
{
    Component* cId   = new CId();
    Component* cType = new CType(ModelType::Cube);
    Component* cTransformable = new CTransformable(10.0,20.0,30.0,    0.0,0.0,0.0,    1.0,1.0,1.0);
    Component* cTexture = new CTexture(std::string("particle.bmp"));
    Component* cMesh   = new CMesh(std::string("media/ninja.b3d"));
    AddComponent(cId);
    AddComponent(cType);
    AddComponent(cTransformable);
    AddComponent(cTexture);
    AddComponent(cMesh);
}


GameObject::GameObject(float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float scaX, float scaY, float scaZ, string texture, string mesh, float maxSpeed, float acceleration , float carFriction, float carSlowDown)
{
    Component* cId   = new CId();
    Component* cType = new CType(ModelType::Cube);
    Component* cTransformable = new CTransformable(posX, posY, posZ, rotX, rotY, rotZ, scaX, scaY, scaZ);
    Component* cTexture = new CTexture(texture);
    Component* cMesh   = new CMesh(mesh);
    Component* cCar = new CCar(maxSpeed,acceleration,carFriction,carSlowDown);
    AddComponent(cId);
    AddComponent(cType);
    AddComponent(cTransformable);
    AddComponent(cTexture);
    AddComponent(cMesh);
    AddComponent(cCar);
}

GameObject::~GameObject()
{
    
}