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
 
 
 #include "Car.h"
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
#include <Components/CAnimation.h>
#include "../Components/CExternalForce.h"
#include "../Components/CBoundingChassis.h"
#include "../Components/CShader.h"
#include "../Components/CGravity.h"
#include "../Components/CBrainAI.h"
#include "../Constants.h"




//LOS DATOS DEL TIPO DEL COCHE -> QUE SE PONGAN SOLO 1 VEZ EN CODIGO
#include "TypeCharacter.h"

Car::Car(int pj){

    mainCharacter _pj;
    string mesh, meshWin, meshRight, meshLeft;
    shared_ptr<CAnimation> anim;
    string texture = "";
    float weight = WEIGHT::W_MEDIUM, maxSpeed = MAX_VELOCITY::V_MEDIUM, acceleration = ACCELERATION::A_MEDIUM/100;
    
    //escala de los coches
    glm::vec3 scale = glm::vec3(6.0f, 6.0f, 6.0f);
    //Movidas de ruedas
    string pathTopLeft,pathTopRight,pathBottomLeft,pathBottomRight;

    glm::vec3 offsetTopLeft,offsetTopRight,offsetBottomLeft,offsetBottomRight;
    glm::vec3 rotationTopLeft,rotationTopRight,rotationBottomLeft,rotationBottomRight;
    glm::vec3 scaleTopLeft,scaleTopRight,scaleBottomLeft,scaleBottomRight;
    float radiusSPhere, distanceSphere, distanceFloor;
    shared_ptr<CType> cType = make_shared<CType>(ModelType::AnimatedMesh);
    
    switch (pj) {
        case 0: {
            _pj =                   mainCharacter::PENGUIN;
            weight =                WEIGHT::W_LOW;
            maxSpeed =              MAX_VELOCITY::V_HIGH;
            acceleration =          ACCELERATION::A_HIGH/100;
            mesh =                  meshCar(PENGUIN);
            anim =                  animCar(PENGUIN);  AddComponent(anim);
            cType = make_shared<CType>(ModelType::StaticMesh);
            meshWin = "";
            meshRight = "";
            meshLeft = "";
 
            pathTopLeft =           "penguin_wheel1.obj";
            pathTopRight =          "penguin_wheel2.obj";
            pathBottomLeft =        "penguin_wheel3.obj";
            pathBottomRight =       "penguin_wheel4.obj";

            offsetTopLeft =         glm::vec3(0.92,0.3f,0.8);
            offsetTopRight =        glm::vec3(0.92,0.3f,-0.8);
            offsetBottomLeft =      glm::vec3(-0.2,0.3f,0.9);
            offsetBottomRight =     glm::vec3(-0.2,0.3f,-0.9);
            rotationTopLeft =       glm::vec3(0.0f,0.0f,0.0f);
            rotationTopRight =      glm::vec3(0.0f,0.0f,0.0f);
            rotationBottomLeft =    glm::vec3(0.0f,0.0f,0.0f);
            rotationBottomRight =   glm::vec3(0.0f,0.0f,0.0f);
            scaleTopLeft =          glm::vec3(1.0f);
            scaleTopRight =         glm::vec3(1.0f);
            scaleBottomLeft =       glm::vec3(1.0f);
            scaleBottomRight =      glm::vec3(1.0f);

            radiusSPhere =          7.0;
            distanceSphere =        2.0;
            distanceFloor =         10.5;

            scale = glm::vec3(7.0f, 7.0f, 7.0f);
        }
            break;
        case 1: 
            _pj =                   mainCharacter::TIGER; 
            weight =                WEIGHT::W_MEDIUM;
            maxSpeed =              MAX_VELOCITY::V_MEDIUM;
            acceleration =          ACCELERATION::A_MEDIUM/100;

            anim =                  animCar(TIGER);  AddComponent(anim);
            cType = make_shared<CType>(ModelType::StaticMesh);


            pathTopLeft =           "mrsbaxter_wheel3.obj";
            pathTopRight =          "mrsbaxter_wheel2.obj";
            pathBottomLeft =        "mrsbaxter_wheel1.obj";
            pathBottomRight =       "mrsbaxter_wheel4.obj";

            offsetTopLeft =         glm::vec3(1.6,0.1f,1.5);
            offsetTopRight =        glm::vec3(1.6,0.1f,-1.5);
            offsetBottomLeft =      glm::vec3(-1.4,0.1f,1.5);
            offsetBottomRight =     glm::vec3(-1.4,0.1f,-1.5);
            rotationTopLeft =       glm::vec3(0.0f,0.0f,0.0f);
            rotationTopRight =      glm::vec3(0.0f,0.0f,0.0f);
            rotationBottomLeft =    glm::vec3(0.0f,0.0f,0.0f);
            rotationBottomRight =   glm::vec3(0.0f,0.0f,0.0f);
            scaleTopLeft =          glm::vec3(1.0f);
            scaleTopRight =         glm::vec3(1.0f);
            scaleBottomLeft =       glm::vec3(1.0f);
            scaleBottomRight =      glm::vec3(1.0f);

            radiusSPhere =          7.5;
            distanceSphere =        4.0;
            distanceFloor =         9.4;

            scale = glm::vec3(5.0f, 5.0f, 5.0f);
            break;
        case 2: 
            _pj =                   mainCharacter::SHARK; 
            weight =                WEIGHT::W_MEDIUM;
            maxSpeed =              MAX_VELOCITY::V_MEDIUM;
            acceleration =          ACCELERATION::A_HIGH/100;
            mesh =                  meshCar(SHARK);
            anim =                  animCar(SHARK);  AddComponent(anim);
            cType = make_shared<CType>(ModelType::StaticMesh);

            pathTopLeft =           "sharky_wheel4.obj";
            pathTopRight =          "sharky_wheel2.obj";
            pathBottomLeft =        "sharky_wheel3.obj";
            pathBottomRight =       "sharky_wheel1.obj";

            offsetTopLeft =         glm::vec3(0.5,0.3f,0.8);
            offsetTopRight =        glm::vec3(0.5,0.3f,-0.8);
            offsetBottomLeft =      glm::vec3(-0.4,0.3f,0.8);
            offsetBottomRight =     glm::vec3(-0.4,0.3f,-0.8);
            rotationTopLeft =       glm::vec3(0.0f,0.0f,0.0f);
            rotationTopRight =      glm::vec3(0.0f,0.0f,0.0f);
            rotationBottomLeft =    glm::vec3(0.0f,0.0f,0.0f);
            rotationBottomRight =   glm::vec3(0.0f,0.0f,0.0f);
            scaleTopLeft =          glm::vec3(1.0f);
            scaleTopRight =         glm::vec3(1.0f);
            scaleBottomLeft =       glm::vec3(1.0f);
            scaleBottomRight =      glm::vec3(1.0f);

            radiusSPhere =          7.0;
            distanceSphere =        2.0;
            distanceFloor =         10.3;

            scale = glm::vec3(8.2f, 8.2f, 8.2f);
            break;
        case 3: 
            _pj =                   mainCharacter::GORILLA; 
            weight =                WEIGHT::W_HIGH;
            maxSpeed =              MAX_VELOCITY::V_MEDIUM;
            acceleration =          ACCELERATION::A_LOW/100;
            mesh =                  meshCar(GORILLA);
            anim =                  animCar(GORILLA);  AddComponent(anim);
            cType = make_shared<CType>(ModelType::StaticMesh);

            pathTopLeft =           "kaiserkong_wheel1.obj";
            pathTopRight =          "kaiserkong_wheel2.obj";
            pathBottomLeft =        "kaiserkong_wheel3.obj";
            pathBottomRight =       "kaiserkong_wheel4.obj";

            offsetTopLeft =         glm::vec3(1.5,0.3f,1.5);
            offsetTopRight =        glm::vec3(1.5,0.3f,-1.5);
            offsetBottomLeft =      glm::vec3(-1.0,0.3f,1.7);
            offsetBottomRight =     glm::vec3(-1.0,0.3f,-1.7);
            rotationTopLeft =       glm::vec3(0.0f,0.0f,0.0f);
            rotationTopRight =      glm::vec3(0.0f,0.0f,0.0f);
            rotationBottomLeft =    glm::vec3(0.0f,0.0f,0.0f);
            rotationBottomRight =   glm::vec3(0.0f,0.0f,0.0f);
            scaleTopLeft =          glm::vec3(1.0f);
            scaleTopRight =         glm::vec3(1.0f);
            scaleBottomLeft =       glm::vec3(1.0f);
            scaleBottomRight =      glm::vec3(1.0f);

            radiusSPhere =          8.2;
            distanceSphere =        4.0;
            distanceFloor =         10.5;

            scale = glm::vec3(5.0f, 5.0f, 5.0f);
            break;
        case 4: 
            _pj =                   mainCharacter::DRAGON; 
            weight =                WEIGHT::W_LOW;
            maxSpeed =              MAX_VELOCITY::V_MEDIUM;
            acceleration =          ACCELERATION::A_HIGH/100;
            mesh =                  meshCar(DRAGON);
            anim =                  animCar(DRAGON);  AddComponent(anim);
            cType = make_shared<CType>(ModelType::StaticMesh);

            pathTopLeft =           "deacon_wheel2.obj";
            pathTopRight =          "deacon_wheel4.obj";
            pathBottomLeft =        "deacon_wheel3.obj";
            pathBottomRight =       "deacon_wheel1.obj";

            offsetTopLeft =         glm::vec3(0.65,0.4f,0.8);
            offsetTopRight =        glm::vec3(0.65,0.4f,-0.9);
            offsetBottomLeft =      glm::vec3(-0.65,0.4f,0.8);
            offsetBottomRight =     glm::vec3(-0.65,0.4f,-0.9);
            rotationTopLeft =       glm::vec3(0.0f,0.0f,0.0f);
            rotationTopRight =      glm::vec3(0.0f,0.0f,0.0f);
            rotationBottomLeft =    glm::vec3(0.0f,0.0f,0.0f);
            rotationBottomRight =   glm::vec3(0.0f,0.0f,0.0f);
            scaleTopLeft =          glm::vec3(1.0f);
            scaleTopRight =         glm::vec3(1.0f);
            scaleBottomLeft =       glm::vec3(1.0f);
            scaleBottomRight =      glm::vec3(1.0f);

            radiusSPhere =          7.0;
            distanceSphere =        2.2;
            distanceFloor =         10.5;

            scale = glm::vec3(7.0f, 7.0f, 7.0f);
            break;
        case 5: 
            _pj =                   mainCharacter::OCTOPUS; 
            weight =                WEIGHT::W_HIGH;
            maxSpeed =              MAX_VELOCITY::V_HIGH;
            acceleration =          ACCELERATION::A_MEDIUM/100;
            mesh =                  meshCar(OCTOPUS);
            anim =                  animCar(OCTOPUS);  AddComponent(anim);
            cType = make_shared<CType>(ModelType::StaticMesh);


            pathTopLeft =           "cyberoctopus_wheel1.obj";
            pathTopRight =          "cyberoctopus_wheel2.obj";
            pathBottomLeft =        "cyberoctopus_wheel3.obj";
            pathBottomRight =       "cyberoctopus_wheel4.obj";

            offsetTopLeft =         glm::vec3(1.0,0.3f,1.3);
            offsetTopRight =        glm::vec3(1.0,0.3f,-1.3);
            offsetBottomLeft =      glm::vec3(-1.2,0.3f,0.62);
            offsetBottomRight =     glm::vec3(-1.2,0.3f,-0.62);

            rotationTopLeft =       glm::vec3(0.0f,0.0f,0.0f);
            rotationTopRight =      glm::vec3(0.0f,0.0f,0.0f);
            rotationBottomLeft =    glm::vec3(0.0f,0.0f,0.0f);
            rotationBottomRight =   glm::vec3(0.0f,0.0f,0.0f);

            scaleTopLeft =          glm::vec3(1.0f);
            scaleTopRight =         glm::vec3(1.0f);
            scaleBottomLeft =       glm::vec3(0.8f);
            scaleBottomRight =      glm::vec3(0.8f);

            radiusSPhere =          7.5;
            distanceSphere =        2.0;
            distanceFloor =         10.0;
    
            scale = glm::vec3(6.3f, 6.3f, 6.3f);
            break;
        default: break;
    }


    // default values
    glm::vec3 pos   = glm::vec3(10.0f, 20.0f, 30.0f);
    glm::vec3 rot   = glm::vec3(0.0f, 90.0f, 0.0f);

    if(Constants::RENDER_ENGINE == Constants::RenderEngine::IRRLICHT){
        mesh    =   "kart_physics.obj";
    }

    string vertexShader = "CLEngine/src/Shaders/lightMapping.vert";
    string fragmentShader = "CLEngine/src/Shaders/lightMapping.frag";

    //string vertexShader = "CLEngine/src/Shaders/cartoonShader.vert";
    //string fragmentShader = "CLEngine/src/Shaders/cartoonShader.frag";

    shared_ptr<CId> cId   = make_shared<CId>();
    shared_ptr<CTransformable> cTransformable = make_shared<CTransformable>(pos, rot, scale); 
    shared_ptr<CTexture> cTexture = make_shared<CTexture>(texture);
    shared_ptr<CMesh> cMesh   = make_shared<CMesh>(mesh);
    shared_ptr<CCar> cCar = make_shared<CCar>(_pj, weight, maxSpeed, acceleration, 10.0);

    shared_ptr<CPowerUp> cPowerUp = make_shared<CPowerUp>();
    shared_ptr<CShield> cShield = make_shared<CShield>();
    shared_ptr<CNitro> cNitro = make_shared<CNitro>();
    shared_ptr<CRoboJorobo> cRoboJorobo = make_shared<CRoboJorobo>();
    shared_ptr<CTotem> cTotem = make_shared<CTotem>();
    shared_ptr<CSpeed> cSpeed = make_shared<CSpeed>();
    shared_ptr<CCurrentNavMesh> cCurrentNavMesh = make_shared<CCurrentNavMesh>(-1);  //  ponemos -1 por defecto ya que haremos el calculo al empezar la partida

    shared_ptr<CColliding> cColliding = make_shared<CColliding>(false);
    shared_ptr<CBoundingSphere> cBoundSphere = make_shared<CBoundingSphere>(pos, radiusSPhere);
    shared_ptr<CBoundingRay> cBoundRay = make_shared<CBoundingRay>();

    glm::vec3 pSphBehind = pos;
    glm::vec3 pSphFront = pos;
    shared_ptr<CBoundingChassis> cBoundingChassis = make_shared<CBoundingChassis>(pSphBehind, pSphFront, radiusSPhere, distanceFloor, distanceSphere);

    shared_ptr<CGravity> cGravity = make_shared<CGravity>();

    shared_ptr<CShader> cShader = make_shared<CShader>(vertexShader,fragmentShader);
    
    shared_ptr<CHurt> cHurt = make_shared<CHurt>();
    shared_ptr<CWheel> cWheel = make_shared<CWheel>(pathTopLeft, pathTopRight, pathBottomLeft, pathBottomRight, offsetTopLeft,offsetTopRight,offsetBottomLeft,offsetBottomRight,rotationTopLeft,rotationTopRight,rotationBottomLeft,rotationBottomRight,scaleTopLeft,scaleTopRight,scaleBottomLeft,scaleBottomRight);

    shared_ptr<CExternalForce> cExternalForce = make_shared<CExternalForce>();


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
    AddComponent(cExternalForce);
    AddComponent(cBoundingChassis);
    AddComponent(cGravity);
    AddComponent(cShader);
    AddComponent(cHurt);
    AddComponent(cWheel);

}