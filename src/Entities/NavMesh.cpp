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
 
 
 #include "NavMesh.h"
#include <Components/CId.h>
#include <Components/CType.h>
#include <Components/CTexture.h>
#include <Components/CMesh.h>
#include <Components/CTransformable.h>
#include <Components/CNavMesh.h>
#include <Components/CDimensions.h>
#include <Components/CShader.h>

#include <iostream>

class Position;
using namespace std;


NavMesh::NavMesh(glm::vec3 pos, glm::vec3 rot, float width, float height, float depth, const vector<int>& waypoints)
{
    string texture = "spheremap.jpg";
    string meshCerca    = "kart.obj";

    string vertexShader = "CLEngine/src/Shaders/lightMapping.vert";
    string fragmentShader = "CLEngine/src/Shaders/lightMapping.frag";

    //string vertexShader = "CLEngine/src/Shaders/cartoonShader.vert";
    //string fragmentShader = "CLEngine/src/Shaders/cartoonShader.frag";

    //Dividimos entre 10 porque con scale 1 irrlicht cuenta como 10, de esta manera si le decimos que queremos 10 de altura lo tomara como 1 y en irrlicht sera 10
    glm::vec3 scale = glm::vec3(width/10.0,height/10.0,depth/10.0);
    
    shared_ptr<CId> cId   = make_shared<CId>();
    shared_ptr<CType> cType = make_shared<CType>(ModelType::Cube);
    shared_ptr<CTransformable> cTransformable = make_shared<CTransformable>(pos, rot, scale); 
    shared_ptr<CTexture> cTexture = make_shared<CTexture>(texture);
    shared_ptr<CMesh> cMesh   = make_shared<CMesh>(meshCerca);
    shared_ptr<CNavMesh> cNavMesh   = make_shared<CNavMesh>(waypoints);
    shared_ptr<CDimensions> cDimensions   = make_shared<CDimensions>(width,height,depth);
    shared_ptr<CShader> cShader = make_shared<CShader>(vertexShader,fragmentShader);


    AddComponent(cId);
    AddComponent(cType);
    AddComponent(cTransformable);
    AddComponent(cTexture);
    AddComponent(cMesh);
    AddComponent(cNavMesh);
    AddComponent(cDimensions);
    AddComponent(cShader);


}




NavMesh::~NavMesh()
{
    
}