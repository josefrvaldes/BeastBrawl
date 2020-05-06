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
#include "GameValues.h"


class Position;
using namespace std;

CarAI::CarAI(int pj){

    typeCar = TypeCar::CarAI;

    mainCharacter _pj;
    string mesh;
    float weight = 50;
    float maxSpeed = 200.0;
    float acceleration = 1.5;
    
    //Movidas de ruedas
    string pathTopLeft;
    string pathTopRight;
    string pathBottomLeft;
    string pathBottomRight;

    glm::vec3 offsetTopLeft;
    glm::vec3 offsetTopRight;
    glm::vec3 offsetBottomLeft;
    glm::vec3 offsetBottomRight;

    glm::vec3 rotationTopLeft;
    glm::vec3 rotationTopRight;
    glm::vec3 rotationBottomLeft;
    glm::vec3 rotationBottomRight;

    glm::vec3 scaleTopLeft;
    glm::vec3 scaleTopRight;
    glm::vec3 scaleBottomLeft;
    glm::vec3 scaleBottomRight;
    
    switch (pj) {
        case 0: 
            _pj = mainCharacter::PENGUIN;
            weight = 30.0;
            maxSpeed = 220.0;
            mesh = "kart_penguin.obj";

            pathTopLeft = "sharky_wheel4.obj";
            pathTopRight = "sharky_wheel2.obj";
            pathBottomLeft = "sharky_wheel3.obj";
            pathBottomRight = "sharky_wheel1.obj";

            offsetTopLeft = glm::vec3(0.6,0.3f,1.0);
            offsetTopRight = glm::vec3(0.6,0.3f,-1.0);
            offsetBottomLeft = glm::vec3(-0.4,0.3f,1.0);
            offsetBottomRight = glm::vec3(-0.4,0.3f,-1.0);

            rotationTopLeft = glm::vec3(0.0f,0.0f,0.0f);
            rotationTopRight = glm::vec3(0.0f,0.0f,0.0f);
            rotationBottomLeft = glm::vec3(0.0f,0.0f,0.0f);
            rotationBottomRight = glm::vec3(0.0f,0.0f,0.0f);

            scaleTopLeft = glm::vec3(1.0f);
            scaleTopRight = glm::vec3(1.0f);
            scaleBottomLeft = glm::vec3(1.0f);
            scaleBottomRight = glm::vec3(1.0f);
            break;
        case 1: 
            _pj = mainCharacter::TIGER; 
            weight = 40.0;
            acceleration = 1.75;
            mesh = "kart_baxter.obj";
            pathTopLeft = "sharky_wheel4.obj";
            pathTopRight = "sharky_wheel2.obj";
            pathBottomLeft = "sharky_wheel3.obj";
            pathBottomRight = "sharky_wheel1.obj";

            offsetTopLeft = glm::vec3(0.6,0.3f,1.0);
            offsetTopRight = glm::vec3(0.6,0.3f,-1.0);
            offsetBottomLeft = glm::vec3(-0.4,0.3f,1.0);
            offsetBottomRight = glm::vec3(-0.4,0.3f,-1.0);

            rotationTopLeft = glm::vec3(0.0f,0.0f,0.0f);
            rotationTopRight = glm::vec3(0.0f,0.0f,0.0f);
            rotationBottomLeft = glm::vec3(0.0f,0.0f,0.0f);
            rotationBottomRight = glm::vec3(0.0f,0.0f,0.0f);

            scaleTopLeft = glm::vec3(1.0f);
            scaleTopRight = glm::vec3(1.0f);
            scaleBottomLeft = glm::vec3(1.0f);
            scaleBottomRight = glm::vec3(1.0f);
            break;
        case 2: 
            _pj = mainCharacter::SHARK; 
            weight = 30.0;
            maxSpeed = 220.0;
            acceleration = 2.0;
            mesh = "kart_sharky.obj";
            pathTopLeft = "sharky_wheel4.obj";
            pathTopRight = "sharky_wheel2.obj";
            pathBottomLeft = "sharky_wheel3.obj";
            pathBottomRight = "sharky_wheel1.obj";

            offsetTopLeft = glm::vec3(0.6,0.3f,1.0);
            offsetTopRight = glm::vec3(0.6,0.3f,-1.0);
            offsetBottomLeft = glm::vec3(-0.4,0.3f,1.0);
            offsetBottomRight = glm::vec3(-0.4,0.3f,-1.0);

            rotationTopLeft = glm::vec3(0.0f,0.0f,0.0f);
            rotationTopRight = glm::vec3(0.0f,0.0f,0.0f);
            rotationBottomLeft = glm::vec3(0.0f,0.0f,0.0f);
            rotationBottomRight = glm::vec3(0.0f,0.0f,0.0f);

            scaleTopLeft = glm::vec3(1.0f);
            scaleTopRight = glm::vec3(1.0f);
            scaleBottomLeft = glm::vec3(1.0f);
            scaleBottomRight = glm::vec3(1.0f);
            break;
        case 3: 
            _pj = mainCharacter::GORILLA; 
            weight = 70.0;
            maxSpeed = 220.0;
            acceleration = 1.25;
            mesh = "kart_kong.obj";
            pathTopLeft = "sharky_wheel4.obj";
            pathTopRight = "sharky_wheel2.obj";
            pathBottomLeft = "sharky_wheel3.obj";
            pathBottomRight = "sharky_wheel1.obj";

            offsetTopLeft = glm::vec3(0.6,0.3f,1.0);
            offsetTopRight = glm::vec3(0.6,0.3f,-1.0);
            offsetBottomLeft = glm::vec3(-0.4,0.3f,1.0);
            offsetBottomRight = glm::vec3(-0.4,0.3f,-1.0);

            rotationTopLeft = glm::vec3(0.0f,0.0f,0.0f);
            rotationTopRight = glm::vec3(0.0f,0.0f,0.0f);
            rotationBottomLeft = glm::vec3(0.0f,0.0f,0.0f);
            rotationBottomRight = glm::vec3(0.0f,0.0f,0.0f);

            scaleTopLeft = glm::vec3(1.0f);
            scaleTopRight = glm::vec3(1.0f);
            scaleBottomLeft = glm::vec3(1.0f);
            scaleBottomRight = glm::vec3(1.0f);
            break;
        case 4: 
            _pj = mainCharacter::DRAGON; 
            acceleration = 2.0;
            mesh = "kart_physics.obj";
            pathTopLeft = "sharky_wheel4.obj";
            pathTopRight = "sharky_wheel2.obj";
            pathBottomLeft = "sharky_wheel3.obj";
            pathBottomRight = "sharky_wheel1.obj";

            offsetTopLeft = glm::vec3(0.6,0.3f,1.0);
            offsetTopRight = glm::vec3(0.6,0.3f,-1.0);
            offsetBottomLeft = glm::vec3(-0.4,0.3f,1.0);
            offsetBottomRight = glm::vec3(-0.4,0.3f,-1.0);

            rotationTopLeft = glm::vec3(0.0f,0.0f,0.0f);
            rotationTopRight = glm::vec3(0.0f,0.0f,0.0f);
            rotationBottomLeft = glm::vec3(0.0f,0.0f,0.0f);
            rotationBottomRight = glm::vec3(0.0f,0.0f,0.0f);

            scaleTopLeft = glm::vec3(1.0f);
            scaleTopRight = glm::vec3(1.0f);
            scaleBottomLeft = glm::vec3(1.0f);
            scaleBottomRight = glm::vec3(1.0f);
            break;
        case 5: 
            _pj = mainCharacter::OCTOPUS; 
            mesh = "kart_physics.obj";
            pathTopLeft = "sharky_wheel4.obj";
            pathTopRight = "sharky_wheel2.obj";
            pathBottomLeft = "sharky_wheel3.obj";
            pathBottomRight = "sharky_wheel1.obj";

            offsetTopLeft = glm::vec3(0.6,0.3f,1.0);
            offsetTopRight = glm::vec3(0.6,0.3f,-1.0);
            offsetBottomLeft = glm::vec3(-0.4,0.3f,1.0);
            offsetBottomRight = glm::vec3(-0.4,0.3f,-1.0);

            rotationTopLeft = glm::vec3(0.0f,0.0f,0.0f);
            rotationTopRight = glm::vec3(0.0f,0.0f,0.0f);
            rotationBottomLeft = glm::vec3(0.0f,0.0f,0.0f);
            rotationBottomRight = glm::vec3(0.0f,0.0f,0.0f);

            scaleTopLeft = glm::vec3(1.0f);
            scaleTopRight = glm::vec3(1.0f);
            scaleBottomLeft = glm::vec3(1.0f);
            scaleBottomRight = glm::vec3(1.0f);
            break;
        default: break;
    }


    // default values
    glm::vec3 pos   = glm::vec3(10.0f, 20.0f, 30.0f);
    glm::vec3 rot   = glm::vec3(0.0f, 90.0f, 0.0f);
    glm::vec3 scale = glm::vec3(6.0f, 6.0f, 6.0f);

     if(Constants::RENDER_ENGINE == Constants::RenderEngine::IRRLICHT){
        mesh    =   "kart_physics.obj";
    }
    
    
    string texture = "";

    string vertexShader = "CLEngine/src/Shaders/cartoonShader.vert";
    string fragmentShader = "CLEngine/src/Shaders/cartoonShader.frag";

    shared_ptr<CId> cId   = make_shared<CId>();
    shared_ptr<CType> cType = make_shared<CType>(ModelType::AnimatedMesh);
    shared_ptr<CTransformable> cTransformable = make_shared<CTransformable>(pos, rot, scale); 
    shared_ptr<CTexture> cTexture = make_shared<CTexture>(texture);
    shared_ptr<CMesh> cMesh   = make_shared<CMesh>(mesh);
    shared_ptr<CCar> cCar = make_shared<CCar>(_pj, weight, maxSpeed, acceleration, 10.0);

    shared_ptr<CWayPoint> cWayPoint = make_shared<CWayPoint>();
    shared_ptr<CPosDestination> cPosDestination = make_shared<CPosDestination>();

    shared_ptr<CPowerUp> cPowerUp = make_shared<CPowerUp>();
    shared_ptr<CShield> cShield = make_shared<CShield>();
    shared_ptr<CNitro> cNitro = make_shared<CNitro>();
    shared_ptr<CRoboJorobo> cRoboJorobo = make_shared<CRoboJorobo>();
    shared_ptr<CTotem> cTotem = make_shared<CTotem>();
    shared_ptr<CSpeed> cSpeed = make_shared<CSpeed>();
    shared_ptr<CCurrentNavMesh> cCurrentNavMesh = make_shared<CCurrentNavMesh>(-1);  //  ponemos -1 por defecto ya que haremos el calculo al empezar la partida


    shared_ptr<CColliding> cColliding = make_shared<CColliding>(false);
    shared_ptr<CBoundingSphere> cBoundSphere = make_shared<CBoundingSphere>(pos);
    shared_ptr<CBoundingRay> cBoundRay = make_shared<CBoundingRay>();

    shared_ptr<CExternalForce> cExternalForce = make_shared<CExternalForce>();

    glm::vec3 pSphBehind = pos;
    glm::vec3 pSphFront = pos;
    shared_ptr<CBoundingChassis> cBoundingChassis = make_shared<CBoundingChassis>(pSphBehind, 7.0, 5.0, pSphFront, 7.0, 5.0);

    shared_ptr<CGravity> cGravity = make_shared<CGravity>();

    shared_ptr<CShader> cShader = make_shared<CShader>(vertexShader,fragmentShader);
    shared_ptr<CBrainAI> cBrainAI = make_shared<CBrainAI>();
    
    shared_ptr<CHurt> cHurt = make_shared<CHurt>();
    shared_ptr<CWheel> cWheel = make_shared<CWheel>(pathTopLeft, pathTopRight, pathBottomLeft, pathBottomRight, offsetTopLeft,offsetTopRight,offsetBottomLeft,offsetBottomRight,rotationTopLeft,rotationTopRight,rotationBottomLeft,rotationBottomRight,scaleTopLeft,scaleTopRight,scaleBottomLeft,scaleBottomRight);


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
    AddComponent(cBoundSphere);
    AddComponent(cBoundRay);
    AddComponent(cColliding);
    AddComponent(cSpeed);

    AddComponent(cCurrentNavMesh);

    AddComponent(cExternalForce);

    AddComponent(cBoundingChassis);

    AddComponent(cGravity);

    AddComponent(cShader);
    AddComponent(cBrainAI);
    AddComponent(cHurt);
    AddComponent(cWheel);
}

CarAI::CarAI(int pj, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale,string texture, string mesh, float maxSpeed, float acceleration , float carFriction, float carSlowDown, std::string vertexShader, std::string fragmentShader)
    : CarAI(pj){

    CTransformable *cTransformable = (CTransformable *)m_components[CompType::TransformableComp].get();
    cTransformable->position = pos;
    cTransformable->rotation = rot;
    cTransformable->scale    = scale;

    if(Constants::RENDER_ENGINE == Constants::RenderEngine::CLOVER){
        cTransformable->rotation.x += 90.0;
    }else if(Constants::RENDER_ENGINE == Constants::RenderEngine::IRRLICHT){

    } 

    

    CTexture *cTexture = (CTexture *)m_components[CompType::TextureComp].get();
    cTexture->texture = texture;

    // DEBUG MESH debería tener 3 mesh en vez de una
    CMesh *cMesh = (CMesh *)m_components[CompType::MeshComp].get();  
    cMesh->mesh.push_back( mesh );

    CCar *cCar = (CCar *)m_components[CompType::CarComp].get();
    cCar->maxSpeed = maxSpeed;
    cCar->acceleration = acceleration;
    cCar->friction = carFriction;
    cCar->slowDown = carSlowDown;

    shared_ptr<CShader> cShader = make_shared<CShader>(vertexShader,fragmentShader);
    AddComponent(cShader);

}



CarAI::CarAI(int pj, glm::vec3 _position) 
    : CarAI(pj)
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
    auto cBrainAI = static_cast<CBrainAI*>(m_components[CompType::BrainAIComp].get());
    cBrainAI->stackPath = path;

}

