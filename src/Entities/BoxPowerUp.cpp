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
 
 
 #include "BoxPowerUp.h"

#include <Components/CId.h>
#include <Components/CType.h>
#include <Components/CTexture.h>
#include <Components/CAnimation.h>
#include <Components/CMesh.h>
#include <Components/CTransformable.h>
#include <Components/CBoxPowerUp.h>
#include "../Components/CBoundingSphere.h"
#include "../Components/CRemovableObject.h"
#include <Components/CShader.h>
#include <Constants.h>

#include <iostream>

class Position;
using namespace std;

BoxPowerUp::BoxPowerUp(){
    // default values
    string texture = "";
    string mesh;
    glm::vec3 scale = vec3(1.5f,1.5f,1.5f);
    if(Constants::RENDER_ENGINE == Constants::RenderEngine::CLOVER){
        mesh    = "powerup.obj";
        //scale = vec3(1.0f,1.0f,1.0f);
        
    }else if(Constants::RENDER_ENGINE == Constants::RenderEngine::IRRLICHT){
        mesh    =   "box_powerup.obj";
    }
    string vertexShader = "CLEngine/src/Shaders/lightMapping.vert";
    string fragmentShader = "CLEngine/src/Shaders/lightMapping.frag";

    //string vertexShader = "CLEngine/src/Shaders/cartoonShader.vert";
    //string fragmentShader = "CLEngine/src/Shaders/cartoonShader.frag";
    //float maxSpeed = 20.0, acceleration = .15, friction = 0.1, slowDown = 0.25;
    
    shared_ptr<CId> cId   = make_shared<CId>();
    shared_ptr<CTransformable> cTransformable = make_shared<CTransformable>(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,0.0f), scale); 
    shared_ptr<CTexture> cTexture = make_shared<CTexture>(texture);

    // animaciones
    // string animationPath = "animations/cyberoctopus/damage/damageoctopus_000001.obj";
    // uint8_t numFrames = 35;
    // // vector<uint8_t> distances {30,30,30,30};
    // shared_ptr<CType> cType = make_shared<CType>(ModelType::StaticMesh);
    // Animation anim{animationPath, numFrames/*, distances*/};
    // shared_ptr<CAnimation> cMesh = make_shared<CAnimation>(anim);

    shared_ptr<CType> cType = make_shared<CType>(ModelType::AnimatedMesh);
    shared_ptr<CMesh> cMesh = make_shared<CMesh>(mesh);


    shared_ptr<CBoxPowerUp> cBoxPowerUp   = make_shared<CBoxPowerUp>();
    shared_ptr<CShader> cShader = make_shared<CShader>(vertexShader,fragmentShader);
    //shared_ptr<CRemovableObject> cRemovableObject = make_shared<CRemovableObject>();
    shared_ptr<CBoundingSphere> cBoundingSphere = make_shared<CBoundingSphere>(vec3(0.0,0.0,0.0), 16.0);

    AddComponent(cId);
    AddComponent(cType);
    AddComponent(cTransformable);
    AddComponent(cTexture);
    AddComponent(cMesh);
    AddComponent(cBoxPowerUp);
    //AddComponent(cRemovableObject); // componente para eliminar la entidad al final y no en medio de la ejecucion
    AddComponent(cBoundingSphere);
    AddComponent(cShader);
    //AddComponent(cCar);



    //cout << "Acabamos de llamar al constructor default de powerup, su transformable es " << cTransformable << endl;

}

BoxPowerUp::BoxPowerUp(glm::vec3 _position) 
    : BoxPowerUp(){
    CTransformable *cTransformable = (CTransformable *)m_components[CompType::TransformableComp].get();
    cTransformable->position.x = _position.x;
    cTransformable->position.y = _position.y;
    cTransformable->position.z = _position.z;

    CBoundingSphere *cSphere = (CBoundingSphere *)m_components[CompType::CompBoundingSphere].get(); 
    cSphere->center =  _position;

    //typePowerUp = _typePowerUp;

    //cout << "Creamos un powerUp del tipo:  " << typePowerUp << endl;
}



BoxPowerUp::~BoxPowerUp(){

}
