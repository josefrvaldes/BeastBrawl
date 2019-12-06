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
    shared_ptr<Component> cId   = make_shared<CId>();
    shared_ptr<Component> cType = make_shared<CType>(ModelType::Cube);
    shared_ptr<Component> cTransformable = make_shared<CTransformable>(glm::vec3(10.0f,20.0f,30.0f),    glm::vec3(0.0f,0.0f,0.0f),    glm::vec3(1.0f,1.0f,1.0f));
    shared_ptr<Component> cTexture = make_shared<CTexture>(std::string("particle.bmp"));
    shared_ptr<Component> cMesh   = make_shared<CMesh>(std::string("media/ninja.b3d"));
    AddComponent(cId);
    AddComponent(cType);
    AddComponent(cTransformable);
    AddComponent(cTexture);
    AddComponent(cMesh);
}


GameObject::GameObject(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, string texture, string mesh)
{
    shared_ptr<Component> cId   = make_shared<CId>();
    shared_ptr<Component> cType = make_shared<CType>(ModelType::Cube);
    shared_ptr<Component> cTransformable = make_shared<CTransformable>(pos, rot, scale);
    shared_ptr<Component> cTexture = make_shared<CTexture>(texture);
    shared_ptr<Component> cMesh   = make_shared<CMesh>(mesh);
    AddComponent(cId);
    AddComponent(cType);
    AddComponent(cTransformable);
    AddComponent(cTexture);
    AddComponent(cMesh);
}

GameObject::~GameObject()
{
    
}