#include "Car.h"
#include "../Components/CPosition.h"
#include "../Components/CSpeed.h"
#include "../Components/CId.h"
#include "../Components/CType.h"
#include "../Components/CTexture.h"
#include "../Components/CMesh.h"
#include "../Components/CTransformable.h"
#include "../Components/CCar.h"
#include <iostream>

class Position;
using namespace std;

Car::Car()
{
    // default values
    glm::vec3 pos   = glm::vec3(10.0f, 20.0f, 30.0f);
    glm::vec3 rot   = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
    string texture = "particle.bmp";
    string mesh    = "media/ninja.b3d";
    float maxSpeed = 20.0, acceleration = .15, friction = 0.1, slowDown = 0.25;
    
    CId* cId   = new CId();
    CType* cType = new CType(ModelType::Cube);
    CTransformable* cTransformable = new CTransformable(pos, rot, scale); 
    CTexture* cTexture = new CTexture(texture);
    CMesh* cMesh   = new CMesh(mesh);
    CCar* cCar = new CCar(maxSpeed, acceleration, friction, slowDown);
    AddComponent(cId);
    AddComponent(cType);
    AddComponent(cTransformable);
    AddComponent(cTexture);
    AddComponent(cMesh);
    AddComponent(cCar);
    cout << "Acabamos de llamar al constructor default de car, su transformable es " << cTransformable << endl;
}

Car::Car(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale,
    string texture, string mesh, 
    float maxSpeed, float acceleration , float carFriction, float carSlowDown) 
{

    
    CId* cId   = new CId();
    CType* cType = new CType(ModelType::Cube);
    CTransformable* cTransformable = new CTransformable(pos, rot, scale); 
    CTexture* cTexture = new CTexture(texture);
    CMesh* cMesh   = new CMesh(mesh);
    CCar* cCar = new CCar(maxSpeed, acceleration, carFriction, carSlowDown);
    AddComponent(cId);
    AddComponent(cType);
    AddComponent(cTransformable);
    AddComponent(cTexture);
    AddComponent(cMesh);
    AddComponent(cCar);
    cout << "Acabamos de llamar al constructor default de car, su transformable es " << cTransformable << endl;


}

Car::~Car()
{
    
}