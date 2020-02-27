#include "Totem.h"
#include <Components/CId.h>
#include <Components/CType.h>
#include <Components/CTexture.h>
#include <Components/CMesh.h>
#include <Components/CTransformable.h>
#include <Components/CCurrentNavMesh.h>
#include "../Components/CBoundingSphere.h"
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
    shared_ptr<CBoundingSphere> cBoundingSphere = make_shared<CBoundingSphere>(vec3(0.0,0.0,0.0), 4.5);
    AddComponent(cId);
    AddComponent(cType);
    AddComponent(cTransformable);
    AddComponent(cTexture);
    AddComponent(cMesh);
    AddComponent(cCurrentNavMesh);
    AddComponent(cBoundingSphere);
}

Totem::Totem(glm::vec3 position_) : Totem(){

    CTransformable *cTransformable = (CTransformable *)m_components[CompType::TransformableComp].get();
    cTransformable->position = position_;

    CBoundingSphere *cBoundingSPhere = (CBoundingSphere *)m_components[CompType::CompBoundingSphere].get();
    cBoundingSPhere->center = position_;
}

Totem::Totem(glm::vec3 position_, glm::vec3 rotation_) : Totem(){

    CTransformable *cTransformable = (CTransformable *)m_components[CompType::TransformableComp].get();
    cTransformable->position = position_;
    cTransformable->rotation = rotation_;

    CBoundingSphere *cBoundingSPhere = (CBoundingSphere *)m_components[CompType::CompBoundingSphere].get();
    cBoundingSPhere->center = position_;
}

Totem::Totem(glm::vec3 position_, glm::vec3 rotation_, string texture_, string mesh_) : Totem(){
    CTransformable *cTransformable = (CTransformable *)m_components[CompType::TransformableComp].get();
    cTransformable->position = position_;
    cTransformable->rotation = rotation_;

    CBoundingSphere *cBoundingSPhere = (CBoundingSphere *)m_components[CompType::CompBoundingSphere].get();
    cBoundingSPhere->center = position_;

    CTexture *cTexture = (CTexture *)m_components[CompType::TextureComp].get();
    cTexture->texture = texture_;

    CMesh *cMesh = (CMesh *)m_components[CompType::MeshComp].get();
    cMesh->mesh = mesh_;

}


//Mi idea es que aqui vayamos comprobando si el totem ha cambiado de navmesh
void Totem::Update(Manager* manNavMesh){

}




Totem::~Totem()
{

}
