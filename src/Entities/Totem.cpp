#include "Totem.h"
#include "../Components/CId.h"
#include "../Components/CType.h"
#include "../Components/CTexture.h"
#include "../Components/CMesh.h"
#include "../Components/CTransformable.h"
#include <iostream>

using namespace std;

Totem::Totem()
{
    // default values
    string texture = "opengllogo.jpg";
    string mesh    = "media/ninja.b3d";
    
    shared_ptr<CId> cId   = make_shared<CId>();
    shared_ptr<CType> cType = make_shared<CType>(ModelType::Cube);
    shared_ptr<CTransformable> cTransformable = make_shared<CTransformable>(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.0f,1.0f,1.0f)); 
    shared_ptr<CTexture> cTexture = make_shared<CTexture>(texture);
    shared_ptr<CMesh> cMesh   = make_shared<CMesh>(mesh);
    AddComponent(cId);
    AddComponent(cType);
    AddComponent(cTransformable);
    AddComponent(cTexture);
    AddComponent(cMesh);
}


Totem::Totem(glm::vec3 _position, glm::vec3 _rotation) : Totem(){

    CTransformable *cTransformable = (CTransformable *)m_components[CompType::TransformableComp].get();
    cTransformable->position = _position;
    cTransformable->rotation = _rotation;
}


Totem::Totem(glm::vec3 _position, glm::vec3 _rotation, string _texture, string _mesh) : Totem(){
    CTransformable *cTransformable = (CTransformable *)m_components[CompType::TransformableComp].get();
    cTransformable->position = _position;
    cTransformable->rotation = _rotation;

    CTexture *cTexture = (CTexture *)m_components[CompType::TextureComp].get();
    cTexture->texture = _texture;

    CMesh *cMesh = (CMesh *)m_components[CompType::MeshComp].get();
    cMesh->mesh = _mesh;

}




Totem::~Totem()
{

}
