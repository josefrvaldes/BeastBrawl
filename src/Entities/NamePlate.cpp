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
 
 
 #include "NamePlate.h"
#include <Components/CId.h>
#include <Components/CNamePlate.h>
#include <Components/CType.h>
#include <Components/CShader.h>

class Position;
using namespace std;


NamePlate::NamePlate(int parentNode,string billboardPath)
{

    string vertexShader = "CLEngine/src/Shaders/lightMapping.vert";
    string fragmentShader = "CLEngine/src/Shaders/lightMapping.frag";

    //string vertexShader = "CLEngine/src/Shaders/cartoonShader.vert";
    //string fragmentShader = "CLEngine/src/Shaders/cartoonShader.frag";

    shared_ptr<Component> cId   = make_shared<CId>();
    shared_ptr<Component> cType   = make_shared<CType>(ModelType::Text);
    shared_ptr<Component> cNamePlate = make_shared<CNamePlate>(parentNode,billboardPath);
    shared_ptr<CShader> cShader = make_shared<CShader>(vertexShader,fragmentShader);

    AddComponent(cId);
    AddComponent(cType);
    AddComponent(cNamePlate);
    AddComponent(cShader);
}

NamePlate::~NamePlate()
{
    
}