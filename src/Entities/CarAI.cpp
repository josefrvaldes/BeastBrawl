#include "CarAI.h"
#include "../Components/CSpeed.h"
#include "../Components/CId.h"
#include "../Components/CType.h"
#include "../Components/CTexture.h"
#include "../Components/CMesh.h"
#include "../Components/CPowerUp.h"
#include "../Components/CTransformable.h"
#include "../Components/CCar.h"
#include "../Components/CWayPoint.h"
#include "../Components/CPath.h"
#include "../Components/CShield.h"
#include "../Components/CTotem.h"
#include "../Components/CRoboJorobo.h"
#include "../Components/CNitro.h"
#include "../Components/CBoundingSphere.h"
#include "../Components/CColliding.h"
#include <iostream>

class Position;
using namespace std;

CarAI::CarAI(){
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


    shared_ptr<CColliding> cColliding = make_shared<CColliding>(false);
    shared_ptr<CBoundingSphere> cBoundSphere = make_shared<CBoundingSphere>(pos);
    AddComponent(cId);
    AddComponent(cType);
    AddComponent(cTransformable);
    AddComponent(cTexture);
    AddComponent(cMesh);
    AddComponent(cCar);

    // TODO quitar el compoentne cWayPoint del coche ya que al coche solo le hace falta su siguiente destino.
    AddComponent(cWayPoint);
    AddComponent(cPosDestination);

    AddComponent(cPowerUp);
    AddComponent(cShield);
    AddComponent(cNitro);
    AddComponent(cRoboJorobo);
    AddComponent(cTotem);
    AddComponent(cPath);
    AddComponent(cBoundSphere);
    AddComponent(cColliding);
    AddComponent(cSpeed);
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
    : CarAI(){

    auto mapTransform = m_components.find(CompType::TransformableComp);
	auto cTransformable = static_cast<CTransformable*>(mapTransform->second.get());
    cTransformable->position = _position;
}



CarAI::~CarAI(){
    
}

// TODO quitarlo ya que nuestro destino no es un wayPoint sino una posicion destino
void CarAI::SetWayPoint(CWayPoint* _waypoint){
    auto cWayPoint = static_cast<CWayPoint*>(m_components[CompType::WayPointComp].get());
    cWayPoint->position = _waypoint->position;
    cWayPoint->id = _waypoint->id;
}

void CarAI::SetDestination(CPosDestination* posDestination){
    auto cPosDestination = static_cast<CPosDestination*>(m_components[CompType::PosDestination].get());
    cPosDestination->position = posDestination->position;
    cPosDestination->id = posDestination->id;
}

void CarAI::SetPath(stack<int> path){
    auto cPath = static_cast<CPath*>(m_components[CompType::PathComp].get());
    cPath->stackPath = path;

}

