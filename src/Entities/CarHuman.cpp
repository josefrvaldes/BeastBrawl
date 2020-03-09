#include "CarHuman.h"

#include <iostream>
#include <Components/CBoundingSphere.h>
#include <Components/CBoundingRay.h>
#include <Components/CCar.h>
#include <Components/CId.h>
#include <Components/CMesh.h>
#include <Components/CPowerUp.h>
#include <Components/CRoboJorobo.h>
#include <Components/CShield.h>
#include <Components/CTexture.h>
#include <Components/CTotem.h>
#include <Components/CTransformable.h>
#include <Components/CType.h>
#include <Components/CSpeed.h>
#include <Components/CColliding.h>
#include <Components/CRoboJorobo.h>
#include <Components/CNitro.h>
#include <Components/CCurrentNavMesh.h>
#include "../Components/COnline.h"
#include "../Components/CExternalForce.h"
#include "../Components/CBoundingChassis.h"
#include "../Components/CGravity.h"
#include "../Components/CShader.h"
#include "../Constants.h"

class Position;

using namespace std;

CarHuman::CarHuman() {
    typeCar = TypeCar::CarHuman;

    // default values
    glm::vec3 pos = glm::vec3(-20.0f, 15.0f, -300.0f);
    glm::vec3 rot = glm::vec3(0.0f, 90.0f, 0.0f);
    glm::vec3 scale = glm::vec3(0.6f, 0.6f, 0.6f);
    string texture = "";

    string mesh;
    if(Constants::RENDER_ENGINE == Constants::RenderEngine::CLOVER){
        mesh    = "kart_ia.obj";
        
    }else if(Constants::RENDER_ENGINE == Constants::RenderEngine::IRRLICHT){
        mesh    =   "kart.obj";
    }

    //string mesh    = "kart_physics.fbx";
    // string mesh    = "kart.obj";

    string vertexShader = "CLEngine/src/Shaders/vertex.glsl";
    string fragmentShader = "CLEngine/src/Shaders/fragment.glsl";
    float maxSpeed = 200.0, acceleration = 1.5, friction = 1.0, slowDown = 2.5;
    
    shared_ptr<CId> cId   = make_shared<CId>();
    shared_ptr<CType> cType = make_shared<CType>(ModelType::AnimatedMesh);
    shared_ptr<CTransformable> cTransformable = make_shared<CTransformable>(pos, rot, scale);
    shared_ptr<CTexture> cTexture = make_shared<CTexture>(texture);
    shared_ptr<CMesh> cMesh = make_shared<CMesh>(mesh);
    shared_ptr<CCar> cCar = make_shared<CCar>(maxSpeed, acceleration, friction, slowDown);
    shared_ptr<CSpeed> cSpeed = make_shared<CSpeed>();
    shared_ptr<CPowerUp> cPowerUp = make_shared<CPowerUp>();
    shared_ptr<CShield> cShield = make_shared<CShield>();
    shared_ptr<CNitro> cNitro = make_shared<CNitro>();
    shared_ptr<CRoboJorobo> cRoboJorobo = make_shared<CRoboJorobo>();
    shared_ptr<CTotem> cTotem = make_shared<CTotem>();
    shared_ptr<CCurrentNavMesh> cCurrentNavMesh = make_shared<CCurrentNavMesh>(-1);  //  ponemos 0 por defecto ya que haremos el calculo al empezar la partida
    shared_ptr<COnline> cOnline = make_shared<COnline>();  //  ponemos 0 por defecto ya que haremos el calculo al empezar la partida
    shared_ptr<CShader> cShader = make_shared<CShader>(vertexShader,fragmentShader);  


    // physics
    shared_ptr<CColliding> cColliding = make_shared<CColliding>(false);
    shared_ptr<CBoundingSphere> cBoundSphere = make_shared<CBoundingSphere>(pos);
    shared_ptr<CBoundingRay> cBoundRay = make_shared<CBoundingRay>();
    shared_ptr<CExternalForce> cExternalForce = make_shared<CExternalForce>();

    //CBoundingChassis(const vec3 &spCenterBehind, const float &spRadiusBehind, const vec3 &spCenterFront, const float &spRadiusFront);
    //     glm::vec3 pos = glm::vec3(-20.0f, 10.0f, -300.0f);
    // TODO: jesuuuuuuus pasame los bounding desde phyton mamoooon JESUS
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
    AddComponent(cOnline);
    AddComponent(cExternalForce);
    AddComponent(cBoundingChassis);
    AddComponent(cGravity);
    AddComponent(cShader);
    //cout << "Acabamos de llamar al constructor default de car, su transformable es " << cTransformable << endl;
}

CarHuman::CarHuman(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale,
         string texture, string mesh,
         float maxSpeed, float acceleration, float carFriction, float carSlowDown,string vertexShader, string fragmentShader) {
    typeCar = TypeCar::CarHuman;
    
    shared_ptr<CId> cId = make_shared<CId>();
    shared_ptr<CType> cType = make_shared<CType>(ModelType::AnimatedMesh);
    shared_ptr<CTransformable> cTransformable = make_shared<CTransformable>(pos, rot, scale);
    shared_ptr<CTexture> cTexture = make_shared<CTexture>(texture);
    shared_ptr<CMesh> cMesh = make_shared<CMesh>(mesh);
    shared_ptr<CCar> cCar = make_shared<CCar>(maxSpeed, acceleration, carFriction, carSlowDown);
    shared_ptr<CPowerUp> cPowerUp = make_shared<CPowerUp>();
    shared_ptr<CShield> cShield = make_shared<CShield>();
    shared_ptr<CNitro> cNitro = make_shared<CNitro>();
    shared_ptr<CRoboJorobo> cRoboJorobo = make_shared<CRoboJorobo>();
    shared_ptr<CColliding> cColliding = make_shared<CColliding>(false);
    shared_ptr<CTotem> cTotem = make_shared<CTotem>();
    shared_ptr<CShader> cShader = make_shared<CShader>(vertexShader,fragmentShader);

    AddComponent(cId);
    AddComponent(cType);
    AddComponent(cTransformable);
    AddComponent(cTexture);
    AddComponent(cMesh);
    AddComponent(cCar);
    AddComponent(cPowerUp);
    AddComponent(cShield);
    AddComponent(cNitro);
    AddComponent(cRoboJorobo);
    AddComponent(cTotem);
    AddComponent(cColliding);
    AddComponent(cShader);
    //cout << "Acabamos de llamar al constructor default de car, su transformable es " << cTransformable << endl;
}

CarHuman::CarHuman(glm::vec3 _position)
    : CarHuman() {
    CTransformable *cTransformable = (CTransformable *)m_components[CompType::TransformableComp].get();
    cTransformable->position.x = _position.x;
    cTransformable->position.y = _position.y;
    cTransformable->position.z = _position.z;
}

CarHuman::~CarHuman() {
}