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
#include <Components/CWheel.h>
#include <Components/CTotem.h>
#include <Components/CTransformable.h>
#include <Components/CType.h>
#include <Components/CSpeed.h>
#include <Components/CColliding.h>
#include <Components/CRoboJorobo.h>
#include <Components/CNitro.h>
#include <Components/CCurrentNavMesh.h>
#include <Components/CHurt.h>
#include "../Components/COnline.h"
#include "../Components/CExternalForce.h"
#include "../Components/CBoundingChassis.h"
#include "../Components/CGravity.h"
#include "../Components/CShader.h"
#include "../Constants.h"
#include "GameValues.h"

class Position;

using namespace std;

CarHuman::CarHuman(int pj) {


    typeCar = TypeCar::CarHuman;


    mainCharacter _pj;
    string mesh;
    string texture = "";
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

            pathTopLeft = "penguin_wheel1.obj";
            pathTopRight = "penguin_wheel2.obj";
            pathBottomLeft = "penguin_wheel3.obj";
            pathBottomRight = "penguin_wheel4.obj";

            offsetTopLeft = glm::vec3(0.92,0.3f,0.8);
            offsetTopRight = glm::vec3(0.92,0.3f,-0.8);
            offsetBottomLeft = glm::vec3(-0.2,0.3f,0.9);
            offsetBottomRight = glm::vec3(-0.2,0.3f,-0.9);

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

            offsetTopLeft = glm::vec3(0.5,0.3f,0.8);
            offsetTopRight = glm::vec3(0.5,0.3f,-0.8);
            offsetBottomLeft = glm::vec3(-0.4,0.3f,0.8);
            offsetBottomRight = glm::vec3(-0.4,0.3f,-0.8);

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
            mesh = "kart_deacon.obj";
            pathTopLeft = "deacon_wheel2.obj";
            pathTopRight = "deacon_wheel4.obj";
            pathBottomLeft = "deacon_wheel3.obj";
            pathBottomRight = "deacon_wheel1.obj";

            offsetTopLeft = glm::vec3(0.65,0.4f,0.8);
            offsetTopRight = glm::vec3(0.65,0.4f,-0.9);
            offsetBottomLeft = glm::vec3(-0.65,0.4f,0.8);
            offsetBottomRight = glm::vec3(-0.65,0.4f,-0.9);

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
    glm::vec3 pos = glm::vec3(-20.0f, 15.0f, -300.0f);
    glm::vec3 rot = glm::vec3(0.0f, 90.0f, 0.0f);
    glm::vec3 scale = glm::vec3(6.0f, 6.0f, 6.0f);

    if(Constants::RENDER_ENGINE == Constants::RenderEngine::IRRLICHT){
        mesh = "kart_physics.obj";
    }

    string vertexShader = "CLEngine/src/Shaders/cartoonShader.vert";
    string fragmentShader = "CLEngine/src/Shaders/cartoonShader.frag";
    
    shared_ptr<CId> cId   = make_shared<CId>();
    shared_ptr<CType> cType = make_shared<CType>(ModelType::AnimatedMesh);
    shared_ptr<CTransformable> cTransformable = make_shared<CTransformable>(pos, rot, scale);
    shared_ptr<CTexture> cTexture = make_shared<CTexture>(texture);
    shared_ptr<CMesh> cMesh = make_shared<CMesh>(mesh);
    shared_ptr<CCar> cCar = make_shared<CCar>(_pj, weight, maxSpeed, acceleration);
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
    shared_ptr<CBoundingChassis> cBoundingChassis = make_shared<CBoundingChassis>(pSphBehind, 7.0, 5.0, pSphFront, 7.0, 5.0);

    shared_ptr<CGravity> cGravity = make_shared<CGravity>();
    shared_ptr<CHurt> cHurt = make_shared<CHurt>();

    shared_ptr<CWheel> cWheel = make_shared<CWheel>(pathTopLeft, pathTopRight, pathBottomLeft, pathBottomRight, offsetTopLeft,offsetTopRight,offsetBottomLeft,offsetBottomRight,rotationTopLeft,rotationTopRight,rotationBottomLeft,rotationBottomRight,scaleTopLeft,scaleTopRight,scaleBottomLeft,scaleBottomRight);

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
    AddComponent(cHurt);
    AddComponent(cWheel);
    //cout << "Acabamos de llamar al constructor default de car, su transformable es " << cTransformable << endl;
}

CarHuman::CarHuman(int pj, glm::vec3 _position)
    : CarHuman(pj) {
    CTransformable *cTransformable = (CTransformable *)m_components[CompType::TransformableComp].get();
    cTransformable->position.x = _position.x;
    cTransformable->position.y = _position.y;
    cTransformable->position.z = _position.z;
}

CarHuman::~CarHuman() {
}