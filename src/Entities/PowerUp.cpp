#include "PowerUp.h"
#include <Components/CId.h>
#include <Components/CType.h>
#include <Components/CTexture.h>
#include <Components/CMesh.h>
#include <Components/CTransformable.h>
#include <Components/CPowerUp.h>
#include <Components/CTargetEntity.h>
#include <iostream>

class Position;
using namespace std;

PowerUp::PowerUp()
{
    // default values
    string texture = "spheremap.jpg";
    string mesh    = "media/ninja.b3d";
    typeCPowerUp typePowerUp = typeCPowerUp::None;
    //float maxSpeed = 20.0, acceleration = .15, friction = 0.1, slowDown = 0.25;
    
    shared_ptr<CId> cId   = make_shared<CId>();
    shared_ptr<CType> cType = make_shared<CType>(ModelType::AnimatedMesh);
    shared_ptr<CTransformable> cTransformable = make_shared<CTransformable>(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.0f,1.0f,1.0f)); 
    shared_ptr<CTexture> cTexture = make_shared<CTexture>(texture);
    shared_ptr<CMesh> cMesh   = make_shared<CMesh>(mesh);
    shared_ptr<CPowerUp> cPowerUp = make_shared<CPowerUp>(typePowerUp);
    shared_ptr<CTargetEntity> cTargetEntity = make_shared<CTargetEntity>();
    AddComponent(cId);
    AddComponent(cType);
    AddComponent(cTransformable);
    AddComponent(cTexture);
    AddComponent(cMesh);
    AddComponent(cPowerUp);
    AddComponent(cTargetEntity);
}


/*PowerUp::PowerUp(glm::vec3 _position) 
    : PowerUp()
{
    CTransformable *cTransformable = (CTransformable *)m_components[CompType::TransformableComp].get();
    cTransformable->position.x = _position.x;
    cTransformable->position.y = _position.y;
    cTransformable->position.z = _position.z;

    //typePowerUp = _typePowerUp;

    //cout << "Creamos un powerUp del tipo:  " << typePowerUp << endl;
}*/



PowerUp::PowerUp(glm::vec3 _position, glm::vec3 _rotation, typeCPowerUp _typePowerUp, CTransformable* cTransformableTarget) : PowerUp(){
    
    CTransformable *cTransformable = (CTransformable *)m_components[CompType::TransformableComp].get();
    cTransformable->position = _position;
    cTransformable->rotation = _rotation;

    CPowerUp *cPowerUp = (CPowerUp *)m_components[CompType::PowerUpComp].get();
    cPowerUp->typePowerUp = _typePowerUp;

    auto *cTarget = (CTargetEntity *)m_components[CompType::TargetEntityComp].get();
    cTarget->cTransTarget = cTransformableTarget;

    CTexture *cTexture = (CTexture *)m_components[CompType::TextureComp].get();
    CMesh *cMesh = (CMesh *)m_components[CompType::MeshComp].get();
    if(_typePowerUp == typeCPowerUp::MelonMolon){           // Melon Molon
        cTexture->texture = "";
        cMesh->mesh = "melon.obj";
        cPowerUp->speed = 510.0;
        cTransformable->scale = glm::vec3(4,4,4);
    }else if(_typePowerUp == typeCPowerUp::PudinDeFrambuesa){   // Pudin Frambuesa
        cTexture->texture = "";
        cMesh->mesh = "pudin.obj";
        cPowerUp->speed = -510.0;
        cTransformable->scale = glm::vec3(4,4,4);
    }else{                                                      // Telebanana
        cTexture->texture = "";
        cMesh->mesh = "telebanana.obj";
        cPowerUp->speed = 375.0;
        cTransformable->scale = glm::vec3(2,2,2);
    }
}


PowerUp::~PowerUp()
{

}
