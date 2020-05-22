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
 
 
 #include "PowerUp.h"
#include <Components/CId.h>
#include <Components/CType.h>
#include <Components/CTexture.h>
#include <Components/CMesh.h>
#include <Components/CTransformable.h>
#include <Components/CBoundingSphere.h>
#include <Components/CPowerUp.h>
#include <Components/CTargetEntity.h>
#include <Components/CShader.h>
#include "../Components/CRemovableObject.h"
#include <iostream>
#include "../Constants.h"

class Position;
using namespace std;

PowerUp::PowerUp()
{
    // default values

    string mesh;
    if(Constants::RENDER_ENGINE == Constants::RenderEngine::CLOVER){
        mesh    = "kart_physics.obj";
        
    }else if(Constants::RENDER_ENGINE == Constants::RenderEngine::IRRLICHT){
        mesh    =   "kart_ia.obj";
    } 
    string texture = "spheremap.jpg";
    //string mesh    = "kart_ia.obj";
    typeCPowerUp typePowerUp = typeCPowerUp::None;

     string vertexShader = "CLEngine/src/Shaders/lightMapping.vert";
     string fragmentShader = "CLEngine/src/Shaders/lightMapping.frag";

    //string vertexShader = "CLEngine/src/Shaders/cartoonShader.vert";
    //string fragmentShader = "CLEngine/src/Shaders/cartoonShader.frag";
    //float maxSpeed = 20.0, acceleration = .15, friction = 0.1, slowDown = 0.25;
    
    shared_ptr<CId> cId   = make_shared<CId>();
    shared_ptr<CType> cType = make_shared<CType>(ModelType::AnimatedMesh);
    shared_ptr<CTransformable> cTransformable = make_shared<CTransformable>(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.6f,1.6f,1.6f)); 
    shared_ptr<CTexture> cTexture = make_shared<CTexture>(texture);
    shared_ptr<CMesh> cMesh   = make_shared<CMesh>(mesh);
    shared_ptr<CPowerUp> cPowerUp = make_shared<CPowerUp>(typePowerUp);
    shared_ptr<CTargetEntity> cTargetEntity = make_shared<CTargetEntity>();
    //shared_ptr<CBoundingSphere> cBoundingSphere = make_shared<CBoundingSphere>(vec3(0.0,0.0,0.0), 4.5);
    shared_ptr<CShader> cShader = make_shared<CShader>(vertexShader,fragmentShader);

    shared_ptr<CRemovableObject> cRemovableObject = make_shared<CRemovableObject>();
    AddComponent(cId);
    AddComponent(cType);
    AddComponent(cTransformable);
    AddComponent(cTexture);
    AddComponent(cMesh);
    AddComponent(cPowerUp);
    AddComponent(cTargetEntity);
    //AddComponent(cBoundingSphere);  // Bounding con el que realmente colisionamos
    AddComponent(cRemovableObject); // componente para eliminar la entidad al final y no a medias de la ejecucion
    AddComponent(cShader);

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

    //CBoundingSphere *cBoundingSphere = (CBoundingSphere *)m_components[CompType::CompBoundingSphere].get();
    //cBoundingSphere->center = _position;

    //cout << "el tipo de powerUp recibido es?¿?¿?¿?¿?¿¿?¿?¿?¿: " << int(_typePowerUp) << endl;

    CPowerUp *cPowerUp = (CPowerUp *)m_components[CompType::PowerUpComp].get();
    cPowerUp->typePowerUp = _typePowerUp;

    //cout << "y lo que metemos dentro en este caso eeees: " << int(cPowerUp->typePowerUp) << endl;

    auto *cTarget = (CTargetEntity *)m_components[CompType::TargetEntityComp].get();
    cTarget->cTransTarget = cTransformableTarget;

    CTexture *cTexture = (CTexture *)m_components[CompType::TextureComp].get();
    CMesh *cMesh = (CMesh *)m_components[CompType::MeshComp].get();
    cMesh->mesh.clear();
    // DEBUG MESH
    if(_typePowerUp == typeCPowerUp::MelonMolon){           // Melon Molon
        cTexture->texture = "";
        cMesh->mesh.push_back("melon.obj");
        cPowerUp->speed = 510.0;
        cTransformable->scale = glm::vec3(1.6f,1.6f,1.6f);
        // cTransformable->scale = glm::vec3(4,4,4);
    }else if(_typePowerUp == typeCPowerUp::PudinDeFrambuesa){   // Pudin Frambuesa
        cTexture->texture = "";
        cMesh->mesh.push_back("pudin.obj");
        cMesh->mesh.push_back("pudin.obj");
        cPowerUp->speed = -510.0;
        cTransformable->scale = glm::vec3(1.4f,1.4f,1.4f);
        // cTransformable->scale = glm::vec3(4,4,4);
    }else{                                                      // Telebanana
        cTexture->texture = "";
        cMesh->mesh.push_back("telebanana.obj");
        cPowerUp->speed = 375.0;
        cTransformable->scale = glm::vec3(1.3f,1.3f,1.3f);
        // cTransformable->scale = glm::vec3(2,2,2);
    }
    cMesh->activeMesh = cMesh->mesh[0];

     string vertexShader = "CLEngine/src/Shaders/lightMapping.vert";
     string fragmentShader = "CLEngine/src/Shaders/lightMapping.frag";

    //string vertexShader = "CLEngine/src/Shaders/cartoonShader.vert";
    //string fragmentShader = "CLEngine/src/Shaders/cartoonShader.frag";

    shared_ptr<CShader> cShader = make_shared<CShader>(vertexShader,fragmentShader);

    AddComponent(cShader);

}


PowerUp::~PowerUp()
{

}
