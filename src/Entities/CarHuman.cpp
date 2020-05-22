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

class Position;

using namespace std;

CarHuman::CarHuman(int pj) : Car(pj) {
    typeCar = TypeCar::CarHuman;

    shared_ptr<COnline> cOnline = make_shared<COnline>();  //  ponemos 0 por defecto ya que haremos el calculo al empezar la partida

    AddComponent(cOnline);


    //cout << "Acabamos de llamar al constructor default de car, su transformable es " << cTransformable << endl;
}

CarHuman::CarHuman(int pj, glm::vec3 _position)
    : CarHuman(pj) {
    CTransformable *cTransformable = (CTransformable *)m_components[CompType::TransformableComp].get();
    cTransformable->position = _position;

    CBoundingChassis *cChassis = (CBoundingChassis *)m_components[CompType::CompBoundingChassis].get();
    cChassis->sphereBehind->center = cTransformable->position;
    cChassis->sphereFront->center = cTransformable->position;
}

CarHuman::~CarHuman() {
}

void CarHuman::SetRotation(glm::vec3 rot_){

    CTransformable *cTransformable = (CTransformable *)m_components[CompType::TransformableComp].get();
    cTransformable->rotation = rot_;
}

void CarHuman::SetPosition(glm::vec3 pos_){

    CTransformable *cTransformable = (CTransformable *)m_components[CompType::TransformableComp].get();
    cTransformable->position = pos_;


    CBoundingChassis *cChassis = (CBoundingChassis *)m_components[CompType::CompBoundingChassis].get();
    cChassis->sphereBehind->center = cTransformable->position;
    cChassis->sphereFront->center = cTransformable->position;
}