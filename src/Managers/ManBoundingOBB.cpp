#include "ManBoundingOBB.h"
#include <functional>
#include <iostream>
#include "../Entities/BoundingWall.h"
#include "../Entities/BoundingOBB.h"
#include "../../include/include_json/include_json.hpp"
#include <fstream>



#include "../Components/CId.h"
#include "../Components/CType.h"
#include "../Components/CTransformable.h"
#include "../Components/CTexture.h"
#include "../Components/CMesh.h"
#include "../Components/CBoundingPlane.h"

#include <Facade/Input/InputFacadeManager.h>
#include <Facade/Physics/PhysicsFacadeManager.h>
#include <Facade/Render/RenderFacadeManager.h>











using namespace std;
using json = nlohmann::json;

ManBoundingOBB::ManBoundingOBB() {
    SubscribeToEvents();



    // 4 Planos normales
    //ManBoundingOBB(vec3(50.f, 40.f, 50.f),vec3(150.f, 40.f, 50.f),vec3(150.f, 10.f, 50.f),vec3(50.f, 10.f, 50.f));
    //ManBoundingOBB(vec3(50.f, 10.f, 150.f),vec3 (150.f, 10.f, 150.f),vec3(150.f, 40.f, 150.f),vec3(50.f, 40.f, 150.f));
    //ManBoundingOBB(vec3(50.f, 10.f, 50.f),vec3(50.f, 10.f, 150.f),vec3(50.f, 40.f, 150.f),vec3(50.f, 40.f, 50.f));
    //ManBoundingOBB(vec3(150.f, 40.f, 50.f),vec3(150.f, 40.f, 150.f),vec3(150.f, 10.f, 150.f),vec3(150.f, 10.f, 50.f));

    //auto verticesOBB = new vector<vec3>;
    //verticesOBB->push_back(vec3(50.f, 40.f, 50.f));
    //verticesOBB->push_back(vec3(150.f, 40.f, 50.f));
    //verticesOBB->push_back(vec3(150.f, -300.f, 50.f));
    //verticesOBB->push_back(vec3(50.f, -300.f, 50.f));
//
    //verticesOBB->push_back(vec3(50.f, -300.f, 150.f));
    //verticesOBB->push_back(vec3 (150.f, -300.f, 150.f));
    //verticesOBB->push_back(vec3(150.f, 40.f, 150.f));
    //verticesOBB->push_back(vec3(50.f, 40.f, 150.f));
//
    //verticesOBB->push_back(vec3(50.f, -300.f, 50.f));
    //verticesOBB->push_back(vec3(50.f, -300.f, 150.f));
    //verticesOBB->push_back(vec3(50.f, 40.f, 150.f));
    //verticesOBB->push_back(vec3(50.f, 40.f, 50.f));
//
    //verticesOBB->push_back(vec3(150.f, 40.f, 50.f));
    //verticesOBB->push_back(vec3(150.f, 40.f, 150.f));
    //verticesOBB->push_back(vec3(150.f, -300.f, 150.f));
    //verticesOBB->push_back(vec3(150.f, -300.f, 50.f));
//
    //vec3 centerMass = vec3( (50+150+50+150+50+50+150+150)/8, (40-300-300+40-300+40-300+40)/8, (50+50+150+150+50+150+50+150)/8);

    //TODO: esssta bien?
    // TODO: CUANDO SE LEAN TENEMOS QUE ASEGURAR QUE LLEGAN 4 PLANOS

    vector<glm::vec3> verticesOBB;
    vector<glm::vec3> centersMass;

    //auto verticesOBB = new vector<glm::vec3>;
    //plano1

    verticesOBB.push_back(glm::vec3(100.f, -100.f, 50.f));
    verticesOBB.push_back(glm::vec3(100.f, -100.f, 150.f));
    verticesOBB.push_back(glm::vec3(100.f, 100.f, 150.f));
    verticesOBB.push_back(glm::vec3(100.f, 100.f, 50.f));

    verticesOBB.push_back(glm::vec3(100.f, 100.f, 50.f));
    verticesOBB.push_back(glm::vec3(150.f, 100.f, 50.f));
    verticesOBB.push_back(glm::vec3(150.f, -100.f, 50.f));
    verticesOBB.push_back(glm::vec3(100.f, -100.f, 50.f));

    verticesOBB.push_back(glm::vec3(100.f, -100.f, 150.f));
    verticesOBB.push_back(glm::vec3(150.f, -100.f, 150.f));
    verticesOBB.push_back(glm::vec3(150.f, 100.f, 150.f));
    verticesOBB.push_back(glm::vec3(100.f, 100.f, 150.f));

    verticesOBB.push_back(glm::vec3(150.f, 100.f, 50.f));
    verticesOBB.push_back(glm::vec3(150.f, 100.f, 150.f));
    verticesOBB.push_back(glm::vec3(150.f, -100.f, 150.f));
    verticesOBB.push_back(glm::vec3(150.f, -100.f, 50.f));

     
    //planes.push_back(make_shared<CBoundingPlane>(*verticesOBB[0], *verticesOBB[1], *verticesOBB[2], *verticesOBB[3]));
    //planes.push_back(make_shared<CBoundingPlane>(verticesOBB[4], verticesOBB[5], verticesOBB[6], verticesOBB[7]));
    //planes.push_back(make_shared<CBoundingPlane>(verticesOBB[8], verticesOBB[9], verticesOBB[10], verticesOBB[11]));
    //planes.push_back(make_shared<CBoundingPlane>(verticesOBB[12], verticesOBB[13], verticesOBB[14], verticesOBB[15]));

    auto centerPlane2   = glm::vec3( (100+150+150+100)/4 , (100+100-100-100)/4 , (50+50+50+50)/4 );
    auto centerPlane1   = glm::vec3( (100+100+100+100)/4 , (100+100-100-100)/4 , (50+150+150+50)/4 );
    auto centerMass     = glm::vec3( (100+150+100+150+100+100+150+150)/8, (100-100-100+100-100+100-100+100)/8, (50+50+150+150+50+150+50+150)/8);
    cout << "el centro de masas es: ( " << centerMass.x << " , " << centerMass.y <<  " , " << centerMass.z << " )" << endl;
    // solo necesitamos 2 planos
    double posX, posX2, posZ, posZ2;
    if( glm::distance(centerPlane1, centerMass) > glm::distance(centerPlane2, centerMass)){
        // el plano 1 por descarte es el pequeño
        auto aumentoZ = abs(verticesOBB[0].x - verticesOBB[2].x)/2;
        auto aumentoX = abs(verticesOBB[0].z - verticesOBB[2].z)/2;
        auto posibleCP1X = glm::vec3(centerPlane1.x+aumentoZ, centerPlane1.y, centerPlane1.z);
        auto posibleCP2X = glm::vec3(centerPlane1.x-aumentoZ, centerPlane1.y, centerPlane1.z);
        if(glm::distance(posibleCP1X, centerMass) > glm::distance(posibleCP2X, centerMass)){
            aumentoZ = -aumentoZ;
        }
        auto posibleCP1Z = glm::vec3(centerPlane1.x, centerPlane1.y, centerPlane1.z+aumentoX);
        auto posibleCP2Z = glm::vec3(centerPlane1.x, centerPlane1.y, centerPlane1.z-aumentoX);
        if(glm::distance(posibleCP1Z, centerMass) > glm::distance(posibleCP2Z, centerMass)){
            aumentoX = -aumentoX;
        }
        // calculamos el otro
        posX = posX2 = centerPlane1.x+aumentoX;
        posZ = posZ2 = centerPlane1.z+aumentoZ;

    }else{
        // el plano 1 por descarte es el pequeño
        auto aumentoZ = abs(verticesOBB[4].x - verticesOBB[6].x)/2;
        auto aumentoX = abs(verticesOBB[4].z - verticesOBB[6].z)/2;
        auto posibleCP1X = glm::vec3(centerPlane2.x+aumentoZ, centerPlane2.y, centerPlane2.z);
        auto posibleCP2X = glm::vec3(centerPlane2.x-aumentoZ, centerPlane2.y, centerPlane2.z);
        if(glm::distance(posibleCP1X, centerMass) > glm::distance(posibleCP2X, centerMass)){
            aumentoZ = -aumentoZ;
        }
        auto posibleCP1Z = glm::vec3(centerPlane2.x, centerPlane2.y, centerPlane2.z+aumentoX);
        auto posibleCP2Z = glm::vec3(centerPlane2.x, centerPlane2.y, centerPlane2.z-aumentoX);
        if(glm::distance(posibleCP1Z, centerMass) > glm::distance(posibleCP2Z, centerMass)){
            aumentoX = -aumentoX;
        }
        // calculamos el otro
        posX = posX2 = centerPlane2.x+aumentoX;
        posZ = posZ2 = centerPlane2.z+aumentoZ;
    }
    auto disX = abs(posX-centerMass.x);
    auto disZ = abs(posZ-centerMass.z);
    if(posX < centerMass.x) posX2 = centerMass.x+disX;
    else posX2 = centerMass.x-disX;   
    if(posZ < centerMass.z) posZ2 = centerMass.z+disZ;
    else posZ2 = centerMass.z-disZ;
    centersMass.push_back(glm::vec3(posX, centerMass.y+30, posZ));
    centersMass.push_back(glm::vec3(posX2, centerMass.y+30, posZ2));




//glm::vec3 rotTotemOnCar1 = glm::vec3(0.0f, 90.0f, 0.0f);
//shared_ptr<Entity> totemOnCar1 = make_shared<Entity>();
//glm::vec3 scaleTotemOnCar1 = glm::vec3(0.5f, 0.5f, 0.5f);
//shared_ptr<CId> cIdTotemOnCar1 = make_shared<CId>();
//shared_ptr<CType> cTypeTotemOnCar1 = make_shared<CType>(ModelType::Cube);
//shared_ptr<CTransformable> cTransformableTotemOnCar1 = make_shared<CTransformable>(centersMass[0], rotTotemOnCar1, scaleTotemOnCar1);
//totemOnCar1->AddComponent(cIdTotemOnCar1);
//totemOnCar1->AddComponent(cTypeTotemOnCar1);
//totemOnCar1->AddComponent(cTransformableTotemOnCar1);
//totemOnCar1->AddComponent(make_shared<CTexture>("totem.jpg"));
//totemOnCar1->AddComponent(make_shared<CMesh>("media/ninja.b3d"));
//
//RenderFacade *renderEngine = RenderFacadeManager::GetInstance()->GetRenderFacade();
//renderEngine->FacadeSuscribeEvents();
//renderEngine->FacadeAddObject(totemOnCar1.get());
//centerPlane1.y += 30;
//shared_ptr<Entity> totemOnCar = make_shared<Entity>();
//glm::vec3 rotTotemOnCar = glm::vec3(0.0f, 90.0f, 0.0f);
//glm::vec3 scaleTotemOnCar= glm::vec3(0.5f, 0.5f, 0.5f);
//shared_ptr<CId> cIdTotemOnCar = make_shared<CId>();
//shared_ptr<CType> cTypeTotemOnCar = make_shared<CType>(ModelType::Cube);
//shared_ptr<CTransformable> cTransformableTotemOnCar = make_shared<CTransformable>(centersMass[1], rotTotemOnCar, scaleTotemOnCar);
//totemOnCar->AddComponent(cIdTotemOnCar);
//totemOnCar->AddComponent(cTypeTotemOnCar);
//totemOnCar->AddComponent(cTransformableTotemOnCar);
//totemOnCar->AddComponent(make_shared<CTexture>("totem.jpg"));
//totemOnCar->AddComponent(make_shared<CMesh>("media/ninja.b3d"));
//renderEngine->FacadeAddObject(totemOnCar.get());


    CreateBoundingOBB(verticesOBB, centersMass );

    centersMass.clear();
    centersMass.shrink_to_fit();
    verticesOBB.clear();
    verticesOBB.shrink_to_fit();
}


void ManBoundingOBB::CreateBoundingOBB(const vector<vec3> &vertices, const vector<vec3> &centersMas){
    shared_ptr<BoundingOBB> p = make_shared<BoundingOBB>(vertices, centersMas);
    entities.push_back(p);
}


ManBoundingOBB::~ManBoundingOBB() {
    cout << "Llamando al destructor de ManBoundingOBB" << endl;
}

void ManBoundingOBB::SubscribeToEvents() {
    // No tiene eventos actualmente
}

void ManBoundingOBB::Integrate(float delta) {
    //No tiene integrate actualmente
}

/*
double ManBoundingOBB::EuclideanDis(const glm::vec3 &p1, const glm::vec3 &p2) const{
    return sqrt(pow((p1.x-p2.x),2)+pow((p1.y-p2.y),2)+pow((p1.z-p2.z),2));
}
*/