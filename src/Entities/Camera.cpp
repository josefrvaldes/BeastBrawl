/**
 * Beast Brawl
 * Game created as a final project of the Multimedia Engineering Degree in the University of Alicante.
 * Made by Clover Games Studio, with members 
 * Carlos de la Fuente Torres delafuentetorresc@gmail.com,
 * Antonio Jose Martinez Garcia https://www.linkedin.com/in/antonio-jose-martinez-garcia/,
 * Jesús Mas Carretero jmasc03@gmail.com, 
 * Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/, 
 * Rubén Rubio Martínez https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/, 
 * and Jose Valdés Sirvent https://www.linkedin.com/in/jose-f-valdés-sirvent-6058b5a5/ github -> josefrvaldes
 * 
 * 
 * @author Clover Games Studio
 * 
 */
 
 
 #include "Camera.h"
#include <Components/CId.h>
#include <Components/CCamera.h>
#include <Components/CShader.h>
#include <Components/CSpeed.h>
#include <Components/CBoundingSphere.h>

class Position;
using namespace std;

Camera::Camera()
{
    string vertexShader = "CLEngine/src/Shaders/lightMapping.vert";
    string fragmentShader = "CLEngine/src/Shaders/lightMapping.frag";

    //string vertexShader = "CLEngine/src/Shaders/cartoonShader.vert";
    //string fragmentShader = "CLEngine/src/Shaders/cartoonShader.frag";

    shared_ptr<Component> cId   = make_shared<CId>();
    shared_ptr<Component> cTransformable = make_shared<CTransformable>(glm::vec3(10.0f,20.0f,30.0f),    glm::vec3(0.0f,0.0f,0.0f),    glm::vec3(1.0f,1.0f,1.0f));
    shared_ptr<Component> cCamera = make_shared<CCamera>(0.0,17.0,0.0,0.0);
    shared_ptr<Component> cShader = make_shared<CShader>(vertexShader,fragmentShader);
    shared_ptr<CSpeed> cSpeed = make_shared<CSpeed>();
    shared_ptr<CBoundingSphere> cBoundSphere = make_shared<CBoundingSphere>(glm::vec3(0.0,0.0,0.0), 15);

    AddComponent(cId);
    AddComponent(cTransformable);
    AddComponent(cCamera);
    AddComponent(cSpeed);
    AddComponent(cShader);
    AddComponent(cBoundSphere);

    SuscribeEvents();
}


Camera::Camera(glm::vec3 pos_, glm::vec3 rot_, glm::vec3 scale_)
    : Camera()
{
    CTransformable *cTransformable = (CTransformable *)m_components[CompType::TransformableComp].get();
    cTransformable->position = pos_;
    cTransformable->rotation = rot_;
    cTransformable->scale = scale_;

}

Camera::Camera(glm::vec3 pos_, glm::vec3 rot_, glm::vec3 scale_, float maxSpeed_, float acc_, float revMaxSpeed_, float slowDown_)
    : Camera()
{
    CTransformable *cTransformable = (CTransformable *)m_components[CompType::TransformableComp].get();
    cTransformable->position = pos_;
    cTransformable->rotation = rot_;
    cTransformable->scale = scale_;

    CCamera *cCamera = (CCamera *)m_components[CompType::CameraComp].get();
    cCamera->ApplyPhysics(maxSpeed_,acc_,revMaxSpeed_, slowDown_);

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

void Camera::setTarget(glm::vec3 tar_){
    auto cCamera = static_cast<CCamera*>(GetComponent(CompType::CameraComp).get());

    cCamera->target = tar_;
}
