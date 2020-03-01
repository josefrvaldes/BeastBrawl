#include "CarAI.h"

#include <Components/CSpeed.h>
#include <Components/CId.h>
#include <Components/CType.h>
#include <Components/CTexture.h>
#include <Components/CMesh.h>
#include <Components/CPowerUp.h>
#include <Components/CTransformable.h>
#include <Components/CCar.h>
#include <Components/CPath.h>
#include <Components/CShield.h>
#include <Components/CTotem.h>
#include <Components/CRoboJorobo.h>
#include <Components/CNitro.h>
#include <Components/CBoundingSphere.h>
#include <Components/CColliding.h>
#include <Components/CCurrentNavMesh.h>
#include <Components/CBoundingRay.h>
#include <Components/CMovementType.h>
#include "../Components/CExternalForce.h"
#include "../Components/CBoundingChassis.h"
#include "../Components/CGravity.h"


class Position;
using namespace std;

CarAI::CarAI(){
    typeCar = TypeCar::CarAI;
    
    // default values
    glm::vec3 pos   = glm::vec3(10.0f, 20.0f, 30.0f);
    glm::vec3 rot   = glm::vec3(0.0f, 90.0f, 0.0f);
    glm::vec3 scale = glm::vec3(0.6f, 0.6f, 0.6f);
    string texture = "";
    string mesh    = "kart_ia.obj";
    float maxSpeed = 200.0, acceleration = 1.5, friction = 1.0, slowDown = 2.5;
    
    shared_ptr<CId> cId   = make_shared<CId>();
    shared_ptr<CType> cType = make_shared<CType>(ModelType::AnimatedMesh);
    shared_ptr<CTransformable> cTransformable = make_shared<CTransformable>(pos, rot, scale); 
    shared_ptr<CTexture> cTexture = make_shared<CTexture>(texture);
    shared_ptr<CMesh> cMesh   = make_shared<CMesh>(mesh);
    shared_ptr<CCar> cCar = make_shared<CCar>(maxSpeed, acceleration, friction, slowDown);

    shared_ptr<CWayPoint> cWayPoint = make_shared<CWayPoint>();
    shared_ptr<CPosDestination> cPosDestination = make_shared<CPosDestination>();

    shared_ptr<CPowerUp> cPowerUp = make_shared<CPowerUp>();
    shared_ptr<CShield> cShield = make_shared<CShield>();
    shared_ptr<CNitro> cNitro = make_shared<CNitro>();
    shared_ptr<CRoboJorobo> cRoboJorobo = make_shared<CRoboJorobo>();
    shared_ptr<CTotem> cTotem = make_shared<CTotem>();
    shared_ptr<CPath> cPath   = make_shared<CPath>();
    shared_ptr<CSpeed> cSpeed = make_shared<CSpeed>();
    shared_ptr<CCurrentNavMesh> cCurrentNavMesh = make_shared<CCurrentNavMesh>(-1);  //  ponemos -1 por defecto ya que haremos el calculo al empezar la partida
    //shared_ptr<CTargetNavMesh> cTargetNavMesh = make_shared<CTargetNavMesh>(-1);  //  ponemos -1 por defecto ya que haremos el calculo al empezar la partida


    shared_ptr<CColliding> cColliding = make_shared<CColliding>(false);
    shared_ptr<CBoundingSphere> cBoundSphere = make_shared<CBoundingSphere>(pos);
    shared_ptr<CBoundingRay> cBoundRay = make_shared<CBoundingRay>();
    shared_ptr<CMovementType> cMovementType = make_shared<CMovementType>("Empty");

    shared_ptr<CExternalForce> cExternalForce = make_shared<CExternalForce>();

    glm::vec3 pSphBehind = pos;
    glm::vec3 pSphFront = pos;
    shared_ptr<CBoundingChassis> cBoundingChassis = make_shared<CBoundingChassis>(pSphBehind, 7.5, pSphFront, 7.5);

    shared_ptr<CGravity> cGravity = make_shared<CGravity>();

    AddComponent(cId);
    AddComponent(cType);
    AddComponent(cTransformable);
    AddComponent(cTexture);
    AddComponent(cMesh);
    AddComponent(cCar);

    // TODO quitar el compoentne cWayPoint del coche ya que al coche solo le hace falta su siguiente destino.
    AddComponent(cWayPoint);
    AddComponent(cPosDestination);
    AddComponent(cMovementType);

    AddComponent(cPowerUp);
    AddComponent(cShield);
    AddComponent(cNitro);
    AddComponent(cRoboJorobo);
    AddComponent(cTotem);
    AddComponent(cPath);
    AddComponent(cBoundSphere);
    AddComponent(cBoundRay);
    AddComponent(cColliding);
    AddComponent(cSpeed);

    AddComponent(cCurrentNavMesh);
    //AddComponent(cTargetNavMesh);

    AddComponent(cExternalForce);

    AddComponent(cBoundingChassis);

    AddComponent(cGravity);
    cout << "Acabamos de llamar al constructor default de car, su transformable es " << cTransformable << endl;
}

CarAI::CarAI(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale,string texture, string mesh, float maxSpeed, float acceleration , float carFriction, float carSlowDown)
    : CarAI(){

    CTransformable *cTransformable = (CTransformable *)m_components[CompType::TransformableComp].get();
    cTransformable->position = pos;
    cTransformable->rotation = rot;
    cTransformable->scale    = scale;

    CTexture *cTexture = (CTexture *)m_components[CompType::TextureComp].get();
    cTexture->texture = texture;

    CMesh *cMesh = (CMesh *)m_components[CompType::MeshComp].get();  
    cMesh->mesh = mesh;

    CCar *cCar = (CCar *)m_components[CompType::CarComp].get();
    cCar->maxSpeed = maxSpeed;
    cCar->acceleration = acceleration;
    cCar->friction = carFriction;
    cCar->slowDown = carSlowDown;
}



CarAI::CarAI(glm::vec3 _position) 
    : CarAI()
{
    CTransformable *cTransformable = (CTransformable *)m_components[CompType::TransformableComp].get();
    cTransformable->position = _position;
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
    auto cPath = static_cast<CPath*>(m_components[CompType::PathComp].get());
    cPath->stackPath = path;

}

