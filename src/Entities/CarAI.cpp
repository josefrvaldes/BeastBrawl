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
 
 
 #include "CarAI.h"

#include <Components/CSpeed.h>
#include <Components/CId.h>
#include <Components/CType.h>
#include <Components/CTexture.h>
#include <Components/CMesh.h>
#include <Components/CPowerUp.h>
#include <Components/CTransformable.h>
#include <Components/CCar.h>
#include <Components/CShield.h>
#include <Components/CTotem.h>
#include <Components/CWheel.h>
#include <Components/CRoboJorobo.h>
#include <Components/CNitro.h>
#include <Components/CBoundingSphere.h>
#include <Components/CColliding.h>
#include <Components/CCurrentNavMesh.h>
#include <Components/CBoundingRay.h>
#include <Components/CHurt.h>
#include "../Components/CExternalForce.h"
#include "../Components/CBoundingChassis.h"
#include "../Components/CShader.h"
#include "../Components/CGravity.h"
#include "../Components/CBrainAI.h"
#include "../Constants.h"



class Position;
using namespace std;

CarAI::CarAI(int pj, int difficult, float timeTotem) : Car(pj){
    typeCar = TypeCar::CarAI;


    shared_ptr<CWayPoint> cWayPoint = make_shared<CWayPoint>();
    shared_ptr<CPosDestination> cPosDestination = make_shared<CPosDestination>();

    BrainAIDifficult dif;
    switch (difficult) {
        case static_cast<int>(BrainAIDifficult::EASY):        dif = BrainAIDifficult::EASY;         break;
        case static_cast<int>(BrainAIDifficult::NORMAL):      dif = BrainAIDifficult::NORMAL;       break;
        case static_cast<int>(BrainAIDifficult::DIFFICULT):   dif = BrainAIDifficult::DIFFICULT;    break;
        default:                                              dif = BrainAIDifficult::NORMAL;       break;
    }
    shared_ptr<CBrainAI> cBrainAI = make_shared<CBrainAI>(dif, timeTotem);


    // TODO: RUBEN quitar el compoentne cWayPoint del coche ya que al coche solo le hace falta su siguiente destino.
    AddComponent(cWayPoint);
    AddComponent(cPosDestination);
    AddComponent(cBrainAI);
}

CarAI::CarAI(int pj, int difficult, float timeTotem, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale,string texture, string mesh, std::string vertexShader, std::string fragmentShader)
    : CarAI(pj, difficult, timeTotem){

    CTransformable *cTransformable = (CTransformable *)m_components[CompType::TransformableComp].get();
    cTransformable->position = pos;
    cTransformable->rotation = rot;
    cTransformable->scale    = scale;

    if(Constants::RENDER_ENGINE == Constants::RenderEngine::CLOVER){
        cTransformable->rotation.x += 90.0;
    }/*else if(Constants::RENDER_ENGINE == Constants::RenderEngine::IRRLICHT){

    } */
    CTexture *cTexture = (CTexture *)m_components[CompType::TextureComp].get();
    cTexture->texture = texture;

    // DEBUG MESH debería tener 3 mesh en vez de una
    CMesh *cMesh = (CMesh *)m_components[CompType::MeshComp].get();  
    cMesh->mesh.push_back( mesh );


    shared_ptr<CShader> cShader = make_shared<CShader>(vertexShader,fragmentShader);
    AddComponent(cShader);

}



CarAI::CarAI(int pj, int difficult, float timeTotem, glm::vec3 _position) 
    : CarAI(pj, difficult, timeTotem)
{
    CTransformable *cTransformable = (CTransformable *)m_components[CompType::TransformableComp].get();
    cTransformable->position = _position;


    CBoundingChassis *cChassis = (CBoundingChassis *)m_components[CompType::CompBoundingChassis].get();
    cChassis->sphereBehind->center = cTransformable->position;
    cChassis->sphereFront->center = cTransformable->position;



}



CarAI::~CarAI(){
    
}

// TODO: quitarlo ya que nuestro destino no es un wayPoint sino una posicion destino
void CarAI::SetWayPoint(CWayPoint* _waypoint){
    auto cWayPoint = static_cast<CWayPoint*>(m_components[CompType::WayPointComp].get());
    cWayPoint->position = _waypoint->position;
    cWayPoint->id = _waypoint->id;
}

void CarAI::SetDestination(CPosDestination* posDestination){
    auto cPosDestination = static_cast<CPosDestination*>(m_components[CompType::PosDestination].get());
    cPosDestination->position = posDestination->position;
    cPosDestination->id = posDestination->id;
    cPosDestination->radious = 10.0f;
}

void CarAI::SetPath(stack<int> path){
    auto cBrainAI = static_cast<CBrainAI*>(m_components[CompType::BrainAIComp].get());
    cBrainAI->stackPath = path;

}


void CarAI::SetRotation(glm::vec3 rot_){
    
    CTransformable *cTransformable = (CTransformable *)m_components[CompType::TransformableComp].get();
    cTransformable->rotation = rot_;
}
