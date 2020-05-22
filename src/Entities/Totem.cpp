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
 
 
 #include "Totem.h"
#include "../Components/CId.h"
#include "../Components/CType.h"
#include "../Components/CTexture.h"
#include "../Components/CMesh.h"
#include "../Components/CTransformable.h"
#include "../Components/CCurrentNavMesh.h"
#include "../Components/CTotem.h"
#include "../Components/CBoundingSphere.h"
#include <Components/CShader.h>
#include <iostream>
#include "../Constants.h"

using namespace std;

Totem::Totem()
{
      // default values
    glm::vec3 pos   = glm::vec3(40.0f, 20.0f, 30.0f);
    glm::vec3 rot   = glm::vec3(0.0f, 90.0f, 0.0f);
    glm::vec3 scale = glm::vec3(0.9f, 0.9f, 0.9f);

    string mesh;
    if(Constants::RENDER_ENGINE == Constants::RenderEngine::CLOVER){
        mesh    = "totem.obj";
        //scale = glm::vec3(1.0f,1.0f,1.0f);
        //mesh  = "TEST_BOX.fbx";
        
    }else if(Constants::RENDER_ENGINE == Constants::RenderEngine::IRRLICHT){
        mesh    =   "totem.obj";
    }

    string texture = " ";
    //string mesh = "totem_tex.fbx";
    //string mesh    = "kart_ia.obj";

    string vertexShader = "CLEngine/src/Shaders/lightMapping.vert";
    string fragmentShader = "CLEngine/src/Shaders/lightMapping.frag";

    //string vertexShader = "CLEngine/src/Shaders/cartoonShader.vert";
    //string fragmentShader = "CLEngine/src/Shaders/cartoonShader.frag";
    
    shared_ptr<CId> cId   = make_shared<CId>();
    shared_ptr<CType> cType = make_shared<CType>(ModelType::AnimatedMesh);
    shared_ptr<CTransformable> cTransformable = make_shared<CTransformable>(pos, rot, scale); 
    shared_ptr<CTexture> cTexture = make_shared<CTexture>(texture);
    shared_ptr<CMesh> cMesh   = make_shared<CMesh>(mesh);
    if(Constants::RENDER_ENGINE == Constants::RenderEngine::CLOVER){
        //cMesh->mesh.push_back("totem_tex.fbx");

    }
    shared_ptr<CCurrentNavMesh> cCurrentNavMesh   = make_shared<CCurrentNavMesh>(-1);
    shared_ptr<CShader> cShader = make_shared<CShader>(vertexShader,fragmentShader);

    shared_ptr<CBoundingSphere> cBoundingSphere = make_shared<CBoundingSphere>(vec3(0.0,0.0,0.0), 7.0);
    shared_ptr<CTotem> cTotem = make_shared<CTotem>();
    AddComponent(cId);
    AddComponent(cType);
    AddComponent(cTransformable);
    AddComponent(cTexture);
    AddComponent(cMesh);
    AddComponent(cCurrentNavMesh);
    AddComponent(cBoundingSphere);
    AddComponent(cShader);
    AddComponent(cTotem);
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

    // DEBUG MESH
    CMesh *cMesh = (CMesh *)m_components[CompType::MeshComp].get();
    cMesh->mesh.push_back( mesh_ );

}


//Mi idea es que aqui vayamos comprobando si el totem ha cambiado de navmesh
void Totem::Update(Manager* manNavMesh){

}




Totem::~Totem()
{

}
