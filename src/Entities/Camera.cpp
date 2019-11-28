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
    Component* cTransformable = new CTransformable(10.0,20.0,30.0,    0.0,0.0,0.0,    1.0,1.0,1.0);
    Component* cCamera = new CCamera(0.0,17.0,0.0,   0.0);
    AddComponent(cId);
    AddComponent(cTransformable);
    AddComponent(cCamera);
}


Camera::Camera(float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float scaX, float scaY, float scaZ)
{
    Component* cId   = new CId();
    Component* cTransformable = new CTransformable(posX, posY+20, posZ, rotX, rotY, rotZ, scaX, scaY, scaZ);
    Component* cCamera = new CCamera(posX,posY+17,posZ,   0.0);
    AddComponent(cId);
    AddComponent(cTransformable);
    AddComponent(cCamera);
}

Camera::~Camera()
{
    
}