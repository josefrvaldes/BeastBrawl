#include "CarAI.h"
#include "../Components/CSpeed.h"
#include "../Components/CId.h"
#include "../Components/CType.h"
#include "../Components/CTexture.h"
#include "../Components/CMesh.h"
#include "../Components/CTransformable.h"
#include "../Components/CCar.h"
#include "../Components/CWayPoint.h"
#include <iostream>

class Position;
using namespace std;

CarAI::CarAI(){
    // default values
    glm::vec3 pos   = glm::vec3(10.0f, 20.0f, 30.0f);
    glm::vec3 rot   = glm::vec3(0.0f, 90.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f, 0.7f, 0.5f);
    string texture = "particle.bmp";
    string mesh    = "media/ninja.b3d";
    float maxSpeed = 20.0, acceleration = 0.15, friction = 0.1, slowDown = 0.25;
    
    shared_ptr<CId> cId   = make_shared<CId>();
    shared_ptr<CType> cType = make_shared<CType>(ModelType::Cube);
    shared_ptr<CTransformable> cTransformable = make_shared<CTransformable>(pos, rot, scale); 
    shared_ptr<CTexture> cTexture = make_shared<CTexture>(texture);
    shared_ptr<CMesh> cMesh   = make_shared<CMesh>(mesh);
    shared_ptr<CCar> cCar = make_shared<CCar>(maxSpeed, acceleration, friction, slowDown);
    shared_ptr<CWayPoint> cWayPoint = make_shared<CWayPoint>();
    AddComponent(cId);
    AddComponent(cType);
    AddComponent(cTransformable);
    AddComponent(cTexture);
    AddComponent(cMesh);
    AddComponent(cCar);
    AddComponent(cWayPoint);
    cout << "Acabamos de llamar al constructor default de car, su transformable es " << cTransformable << endl;
}

CarAI::CarAI(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale,
    string texture, string mesh, 
    float maxSpeed, float acceleration , float carFriction, float carSlowDown) 
{

    
    shared_ptr<CId> cId   = make_shared<CId>();
    shared_ptr<CType> cType = make_shared<CType>(ModelType::Cube);
    shared_ptr<CTransformable> cTransformable = make_shared<CTransformable>(pos, rot, scale); 
    shared_ptr<CTexture> cTexture = make_shared<CTexture>(texture);
    shared_ptr<CMesh> cMesh   = make_shared<CMesh>(mesh);
    shared_ptr<CCar> cCar = make_shared<CCar>(maxSpeed, acceleration, carFriction, carSlowDown);
    AddComponent(cId);
    AddComponent(cType);
    AddComponent(cTransformable);
    AddComponent(cTexture);
    AddComponent(cMesh);
    AddComponent(cCar);
    cout << "Acabamos de llamar al constructor default de car, su transformable es " << cTransformable << endl;


}



CarAI::CarAI(glm::vec3 _position) 
    : CarAI()
{

    //auto components = car->GetComponents();
    auto mapTransform = m_components.find(CompType::TransformableComp);
	auto cTransformable = static_cast<CTransformable*>(mapTransform->second.get());
    cTransformable->position = _position;
}



CarAI::~CarAI()
{
    
}


void CarAI::SetWayPoint(glm::vec3 waypoint){
    auto mapTransform = m_components.find(CompType::WayPointComp);
    auto cWayPoint = static_cast<CWayPoint*>(m_components[CompType::WayPointComp].get());
    cWayPoint->position = waypoint;
    
}
