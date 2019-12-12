#include "PowerUp.h"
#include "../Components/CSpeed.h"
#include "../Components/CId.h"
#include "../Components/CType.h"
#include "../Components/CTexture.h"
#include "../Components/CMesh.h"
#include "../Components/CTransformable.h"
#include "../Components/CCar.h"
#include "../Components/CPowerUp.h"
#include <iostream>

class Position;
using namespace std;

PowerUp::PowerUp()
{
    // default values
    string texture = "earth.jpg";
    string mesh    = "media/ninja.b3d";
    typeCPowerUp typePowerUp = typeCPowerUp::None;
    //float maxSpeed = 20.0, acceleration = .15, friction = 0.1, slowDown = 0.25;
    
    shared_ptr<CId> cId   = make_shared<CId>();
    shared_ptr<CType> cType = make_shared<CType>(ModelType::Cube);
    shared_ptr<CTransformable> cTransformable = make_shared<CTransformable>(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.0f,1.0f,1.0f)); 
    shared_ptr<CTexture> cTexture = make_shared<CTexture>(texture);
    shared_ptr<CMesh> cMesh   = make_shared<CMesh>(mesh);
    shared_ptr<CPowerUp> cPowerUp = make_shared<CPowerUp>(typePowerUp);
    AddComponent(cId);
    AddComponent(cType);
    AddComponent(cTransformable);
    AddComponent(cTexture);
    AddComponent(cMesh);
    AddComponent(cPowerUp);
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



PowerUp::PowerUp(glm::vec3 _position, glm::vec3 _rotation, typeCPowerUp _typePowerUp) : PowerUp(){

    CTransformable *cTransformable = (CTransformable *)m_components[CompType::TransformableComp].get();
    cTransformable->position = _position;
    cTransformable->rotation = _rotation;

    CPowerUp *cPowerUp = (CPowerUp *)m_components[CompType::PowerUpComp].get();
    cPowerUp->typePowerUp = _typePowerUp;
}


PowerUp::PowerUp(glm::vec3 _position, glm::vec3 _rotation, typeCPowerUp _typePowerUp, string _texture, string _mesh) : PowerUp(){
    CTransformable *cTransformable = (CTransformable *)m_components[CompType::TransformableComp].get();
    cTransformable->position = _position;
    cTransformable->rotation = _rotation;

    CPowerUp *cPowerUp = (CPowerUp *)m_components[CompType::PowerUpComp].get();
    cPowerUp->typePowerUp = _typePowerUp;

    CTexture *cTexture = (CTexture *)m_components[CompType::TextureComp].get();
    cTexture->texture = _texture;

    CMesh *cMesh = (CMesh *)m_components[CompType::MeshComp].get();
    cMesh->mesh = _mesh;

}




PowerUp::~PowerUp()
{

}
