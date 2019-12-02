#include "Camera.h"
#include "../Components/CId.h"
#include "../Components/CTransformable.h"
#include "../Components/CCamera.h"
#include "iostream"

class Position;
using namespace std;

Camera::Camera()
{
    Component* cId   = new CId();
    Component* cTransformable = new CTransformable(glm::vec3(10.0f,20.0f,30.0f),    glm::vec3(0.0f,0.0f,0.0f),    glm::vec3(1.0f,1.0f,1.0f));
    Component* cCamera = new CCamera(0.0,17.0,0.0,   0.0);
    AddComponent(cId);
    AddComponent(cTransformable);
    AddComponent(cCamera);
}


Camera::Camera(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
{
    Component* cId   = new CId();
    Component* cTransformable = new CTransformable(pos,rot,scale);
    //Component* cTransformable = new CTransformable(posX, posY+20, posZ, rotX, rotY, rotZ, scaX, scaY, scaZ);
    Component* cCamera = new CCamera(pos.x,pos.y+17,pos.z,   0.0);
    AddComponent(cId);
    AddComponent(cTransformable);
    AddComponent(cCamera);
}

Camera::~Camera()
{
    
}