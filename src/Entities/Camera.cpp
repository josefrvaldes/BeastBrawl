#include "Camera.h"
#include <Components/CId.h>
#include <Components/CCamera.h>

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

    SuscribeEvents();
}

Camera::~Camera()
{
    
}

void Camera::SuscribeEvents(){
    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::INVERT_CAMERA,
        bind(&Camera::InvertCamera, this, placeholders::_1),
        "InvertCamera"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::TOTEM_CAMERA,
        bind(&Camera::TotemCamera, this, placeholders::_1),
        "TotemCamera"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::NORMAL_CAMERA,
        bind(&Camera::NormalCamera, this, placeholders::_1),
        "NormalCamera"));
}


void Camera::InvertCamera(DataMap* d){ 
    auto cCamera = static_cast<CCamera*>(GetComponent(CompType::CameraComp).get());

    //Si esta invertida la desinvertimos y viceversa

    if(cCamera->camType == CamType::INVERTED_CAM){
        cCamera->camType = CamType::NORMAL_CAM;
    }else{
        cCamera->camType = CamType::INVERTED_CAM;

    }
}

void Camera::TotemCamera(DataMap* d){      
    auto cCamera = static_cast<CCamera*>(GetComponent(CompType::CameraComp).get());

    if(cCamera->camType == CamType::TOTEM_CAM){
        cCamera->camType = CamType::NORMAL_CAM;
    }else{
        cCamera->camType = CamType::TOTEM_CAM;

    }
}

void Camera::NormalCamera(DataMap* d){
    auto cCamera = static_cast<CCamera*>(GetComponent(CompType::CameraComp).get());

    cCamera->camType = CamType::NORMAL_CAM;

}

