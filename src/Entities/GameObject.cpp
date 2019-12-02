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
    Component* cTransformable = new CTransformable(glm::vec3(10.0f,20.0f,30.0f),    glm::vec3(0.0f,0.0f,0.0f),    glm::vec3(1.0f,1.0f,1.0f));
    Component* cTexture = new CTexture(std::string("particle.bmp"));
    Component* cMesh   = new CMesh(std::string("media/ninja.b3d"));
    AddComponent(cId);
    AddComponent(cType);
    AddComponent(cTransformable);
    AddComponent(cTexture);
    AddComponent(cMesh);
}


GameObject::GameObject(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, string texture, string mesh)
{
    Component* cId   = new CId();
    Component* cType = new CType(ModelType::Cube);
    Component* cTransformable = new CTransformable(pos, rot, scale);
    Component* cTexture = new CTexture(texture);
    Component* cMesh   = new CMesh(mesh);
    AddComponent(cId);
    AddComponent(cType);
    AddComponent(cTransformable);
    AddComponent(cTexture);
    AddComponent(cMesh);
}

GameObject::~GameObject()
{
    
}