#include "Camera.h"
#include "../Components/CId.h"
#include "../Components/CTransformable.h"
#include "../Components/CCamera.h"
#include "iostream"

class Position;
using namespace std;

Camera::Camera()
{
    shared_ptr<Component> cId   = make_shared<CId>();
    shared_ptr<Component> cTransformable = make_shared<CTransformable>(glm::vec3(10.0f,20.0f,30.0f),    glm::vec3(0.0f,0.0f,0.0f),    glm::vec3(1.0f,1.0f,1.0f));
    shared_ptr<Component> cCamera = make_shared<CCamera>(0.0,17.0,0.0,   0.0);
    AddComponent(cId);
    AddComponent(cTransformable);
    AddComponent(cCamera);
}


Camera::Camera(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
{
    shared_ptr<Component> cId   = make_shared<CId>();
    shared_ptr<Component> cTransformable = make_shared<CTransformable>(pos,rot,scale);
    shared_ptr<Component> cCamera = make_shared<CCamera>(pos.x,pos.y+17,pos.z,   0.0);
    AddComponent(cId);
    AddComponent(cTransformable);
    AddComponent(cCamera);
}

Camera::~Camera()
{
    
}