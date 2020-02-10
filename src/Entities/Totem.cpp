#include "Totem.h"
#include <Components/CId.h>
#include <Components/CType.h>
#include <Components/CTexture.h>
#include <Components/CMesh.h>
#include <Components/CTransformable.h>
#include <Components/CCurrentNavMesh.h>
#include <iostream>

using namespace std;

Totem::Totem()
{
      // default values
    glm::vec3 pos   = glm::vec3(40.0f, 20.0f, 30.0f);
    glm::vec3 rot   = glm::vec3(0.0f, 90.0f, 0.0f);
    glm::vec3 scale = glm::vec3(0.6f, 0.6f, 0.6f);

    string texture = "totem.jpg";
    string mesh    = "media/ninja.b3d";
    
    shared_ptr<CId> cId   = make_shared<CId>();
    shared_ptr<CType> cType = make_shared<CType>(ModelType::Cube);
    shared_ptr<CTransformable> cTransformable = make_shared<CTransformable>(pos, rot, scale); 
    shared_ptr<CTexture> cTexture = make_shared<CTexture>(texture);
    shared_ptr<CMesh> cMesh   = make_shared<CMesh>(mesh);
    shared_ptr<CCurrentNavMesh> cCurrentNavMesh   = make_shared<CCurrentNavMesh>(-1);
    AddComponent(cId);
    AddComponent(cType);
    AddComponent(cTransformable);
    AddComponent(cTexture);
    AddComponent(cMesh);
    AddComponent(cCurrentNavMesh);
}

Totem::Totem(glm::vec3 _position) : Totem(){

    CTransformable *cTransformable = (CTransformable *)m_components[CompType::TransformableComp].get();
    cTransformable->position = _position;
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


//Mi idea es que aqui vayamos comprobando si el totem ha cambiado de navmesh
void Totem::Update(Manager* manNavMesh){

}




Totem::~Totem()
{

}
