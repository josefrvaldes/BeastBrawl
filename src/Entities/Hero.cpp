#include "Hero.h"
#include "../Components/CPosition.h"
#include "../Components/CSpeed.h"
#include "../Components/CId.h"
#include "../Components/CType.h"
#include "../Components/CTexture.h"
#include "../Components/CMesh.h"
#include "../Components/CTransformable.h"
#include "iostream"

class Position;
using namespace std;

Hero::Hero()
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


Hero::Hero(float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float scaX, float scaY, float scaZ)
{
    Component* cId   = new CId();
    Component* cType = new CType(ModelType::Cube);
    Component* cTransformable = new CTransformable(posX, posY, posZ, rotX, rotY, rotZ, scaX, scaY, scaZ);
    Component* cTexture = new CTexture(std::string("particle.bmp"));
    Component* cMesh   = new CMesh(std::string("media/ninja.b3d"));
    AddComponent(cId);
    AddComponent(cType);
    AddComponent(cTransformable);
    AddComponent(cTexture);
    AddComponent(cMesh);
}

Hero::~Hero()
{
    
}