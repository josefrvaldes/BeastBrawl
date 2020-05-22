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
 
 
 #include "Light.h"

#include <Components/CId.h>
#include <Components/CTransformable.h>
#include <Components/CShader.h>
#include <Components/CLight.h>
#include <Components/CType.h>
#include <iostream>

class Position;
using namespace std;

Light::Light(glm::vec3 pos)
{
    string vertexShader = "CLEngine/src/Shaders/lightMapping.vert";
    string fragmentShader = "CLEngine/src/Shaders/lightMapping.frag";

    //string vertexShader = "CLEngine/src/Shaders/cartoonShader.vert";
    //string fragmentShader = "CLEngine/src/Shaders/cartoonShader.frag";

    shared_ptr<Component> cId   = make_shared<CId>();
    shared_ptr<Component> cTransformable = make_shared<CTransformable>(pos,    glm::vec3(0.0f,0.0f,0.0f),    glm::vec3(1.0f,1.0f,1.0f));
    shared_ptr<CType> cType   = make_shared<CType>(ModelType::PointLight);
    shared_ptr<CShader> cShader = make_shared<CShader>(vertexShader,fragmentShader);
    shared_ptr<CLight> cLight   = make_shared<CLight>();

    AddComponent(cId);
    AddComponent(cTransformable);
    AddComponent(cType);
    AddComponent(cShader);
    AddComponent(cLight); 

}


Light::Light(glm::vec3 pos, glm::vec3 intensity, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic)
{

    string vertexShader = "CLEngine/src/Shaders/lightMapping.vert";
    string fragmentShader = "CLEngine/src/Shaders/lightMapping.frag";

    shared_ptr<Component> cId   = make_shared<CId>();
    shared_ptr<Component> cTransformable = make_shared<CTransformable>(pos, glm::vec3(0.0f,0.0f,0.0f),    glm::vec3(1.0f,1.0f,1.0f));
    shared_ptr<CType> cType   = make_shared<CType>(ModelType::PointLight);
    shared_ptr<CShader> cShader = make_shared<CShader>(vertexShader,fragmentShader);
    shared_ptr<CLight> cLight   = make_shared<CLight>(intensity,ambient,diffuse,specular,constant,linear,quadratic);

    AddComponent(cId);
    AddComponent(cTransformable);
    AddComponent(cType);
    AddComponent(cShader);
    AddComponent(cLight);
}

Light::~Light()
{
    
}