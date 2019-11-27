#include "ObstacleGround.h"
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

ObstacleGround::ObstacleGround()
{
    cout << "Soy el ObstacleGround y compilo" << endl;
    cout << "Me voy a poner un componente para probar" << endl;
    Component* cId   = new CId();
    Component* cType = new CType(ModelType::Cube);
    Component* cTransformable = new CTransformable(10.0,10.0,30.0,    0.0,0.0,0.0,    100.0,1.0,100.0);
    Component* cTexture = new CTexture(std::string("wall.jpg"));
    Component* cMesh   = new CMesh(std::string("media/ninja.b3d"));
    AddComponent(cId);
    AddComponent(cType);
    AddComponent(cTransformable);
    AddComponent(cTexture);
    AddComponent(cMesh);
}


ObstacleGround::ObstacleGround(float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float scaX, float scaY, float scaZ)
{
    cout << "Soy el ObstacleGround y compilo" << endl;
    cout << "Me voy a poner un componente para probar" << endl;
    Component* cId   = new CId();
    Component* cType = new CType(ModelType::Cube);
    Component* cTransformable = new CTransformable(posX, posY, posZ, rotX, rotY, rotZ, scaX, scaY, scaZ);
    Component* cTexture = new CTexture(std::string("wall.jpg"));
    Component* cMesh   = new CMesh(std::string("media/ninja.b3d"));
    AddComponent(cId);
    AddComponent(cType);
    AddComponent(cTransformable);
    AddComponent(cTexture);
    AddComponent(cMesh);
}

ObstacleGround::~ObstacleGround()
{
    
}