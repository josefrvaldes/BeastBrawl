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
 
 
 #include "GameObject.h"

#include <Components/CId.h>
#include <Components/CType.h>
#include <Components/CTexture.h>
#include <Components/CMesh.h>
#include <Components/CTransformable.h>
#include <Components/CShader.h>
#include <iostream>

class Position;
using namespace std;

GameObject::GameObject()
{
    shared_ptr<Component> cId   = make_shared<CId>();
    shared_ptr<Component> cType = make_shared<CType>(ModelType::AnimatedMesh);
    shared_ptr<Component> cTransformable = make_shared<CTransformable>(glm::vec3(10.0f,20.0f,30.0f),    glm::vec3(0.0f,0.0f,0.0f),    glm::vec3(1.0f,1.0f,1.0f));
    shared_ptr<Component> cTexture = make_shared<CTexture>(std::string("particle.bmp"));
    shared_ptr<Component> cMesh   = make_shared<CMesh>(std::string("kart_ia.obj"));
    AddComponent(cId);
    AddComponent(cType);
    AddComponent(cTransformable);
    AddComponent(cTexture);
    AddComponent(cMesh);
}


GameObject::GameObject(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, const string& texture, const string& mesh)
{

    string vertexShader = "CLEngine/src/Shaders/lightMapping.vert";
    string fragmentShader = "CLEngine/src/Shaders/lightMapping.frag";

    //string vertexShader = "CLEngine/src/Shaders/cartoonShader.vert";
    //string fragmentShader = "CLEngine/src/Shaders/cartoonShader.frag";

    shared_ptr<Component> cId   = make_shared<CId>();
    shared_ptr<Component> cType = make_shared<CType>(ModelType::AnimatedMesh);
    shared_ptr<Component> cTransformable = make_shared<CTransformable>(pos, rot, scale);
    shared_ptr<Component> cTexture = make_shared<CTexture>(texture);
    shared_ptr<Component> cMesh   = make_shared<CMesh>(mesh);
    shared_ptr<CShader> cShader = make_shared<CShader>(vertexShader,fragmentShader);

    AddComponent(cId);
    AddComponent(cType);
    AddComponent(cTransformable);
    AddComponent(cTexture);
    AddComponent(cMesh);
    AddComponent(cShader);
}

GameObject::~GameObject()
{
    
}