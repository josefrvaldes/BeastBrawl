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

    SuscribeEvents();
}

Camera::~Camera()
{
    
}

void Camera::SuscribeEvents(){
    EventManager::GetInstance().SuscribeMulti(Listener(
        EventType::INVERT_CAMERA,
        bind(&Camera::InvertCamera, this, placeholders::_1),
        "InvertCamera"));

    EventManager::GetInstance().SuscribeMulti(Listener(
        EventType::TOTEM_CAMERA,
        bind(&Camera::TotemCamera, this, placeholders::_1),
        "TotemCamera"));

    EventManager::GetInstance().SuscribeMulti(Listener(
        EventType::NORMAL_CAMERA,
        bind(&Camera::NormalCamera, this, placeholders::_1),
        "NormalCamera"));
}


void Camera::InvertCamera(DataMap d){ 
    auto cCamera = static_cast<CCamera*>(GetComponent(CompType::CameraComp).get());

    //Si esta invertida la desinvertimos y viceversa

    if(cCamera->camType == CamType::INVERTED){
        cCamera->camType = CamType::NORMAL;
    }else{
        cCamera->camType = CamType::INVERTED;

    }
}

void Camera::TotemCamera(DataMap d){      
    auto cCamera = static_cast<CCamera*>(GetComponent(CompType::CameraComp).get());

    if(cCamera->camType == CamType::TOTEM){
        cCamera->camType = CamType::NORMAL;
    }else{
        cCamera->camType = CamType::TOTEM;

    }
}

void Camera::NormalCamera(DataMap d){
    auto cCamera = static_cast<CCamera*>(GetComponent(CompType::CameraComp).get());

    cCamera->camType = CamType::NORMAL;

}

