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

    double vertex1X = 0, vertex1Y = 0, vertex1Z = 0;  // utilizamos double porque tiene mas precison que float (64b vs 32b)
    double vertex2X = 0, vertex2Y = 0, vertex2Z = 0;
    double vertex3X = 0, vertex3Y = 0, vertex3Z = 0;
    double vertex4X = 0, vertex4Y = 0, vertex4Z = 0;

    std::ifstream i("data.json");
    std::stringstream buffer;
    buffer << i.rdbuf();
    string jsonString = buffer.str();
    json j = json::parse(jsonString);
    i.close();



    vector<glm::vec3> verticesOBB;
    vector<glm::vec3> centersMass;
    glm::vec3 centerPlane1  = glm::vec3(0.0,0.0,0.0);
    glm::vec3 centerPlane2  = glm::vec3(0.0,0.0,0.0);
    glm::vec3 centerMass    = glm::vec3(0.0,0.0,0.0); //     = glm::vec3( (100+150+100+150+100+100+150+150)/8, (100-100-100+100-100+100-100+100)/8, (50+50+150+150+50+150+50+150)/8);
    double posX, posX2, posZ, posZ2;
    long unsigned int obbCount = j["OBBS"].size();
    //std::cout << "EL NUMERO DE OBBs EN EL JSON ES: " << obbCount << std::endl;
    //Leemos el array de NavMesh
    for(long unsigned int i = 0; i< obbCount; ++i){
        string idObb = to_string(i);  
        auto obbActual = j["OBBS"][idObb];

        verticesOBB.clear();
        verticesOBB.shrink_to_fit();
        centersMass.clear();
        centersMass.shrink_to_fit();
        centerMass = glm::vec3(0.0,0.0,0.0);

        for(long unsigned int k=1; k<5; k++){
            auto planeObbActual = obbActual["face"+to_string(k)+""];


            // cogemos los 4 vertices
            auto vertex1 = planeObbActual["vertex1"];
            vertex1X = vertex1["x"].get<double>();
            vertex1Y = vertex1["y"].get<double>();
            vertex1Z = vertex1["z"].get<double>();

            auto vertex2 = planeObbActual["vertex2"];
            vertex2X = vertex2["x"].get<double>();
            vertex2Y = vertex2["y"].get<double>();
            vertex2Z = vertex2["z"].get<double>();

            auto vertex3 = planeObbActual["vertex3"];
            vertex3X = vertex3["x"].get<double>();
            vertex3Y = vertex3["y"].get<double>();
            vertex3Z = vertex3["z"].get<double>();

            auto vertex4 = planeObbActual["vertex4"];
            vertex4X = vertex4["x"].get<double>();
            vertex4Y = vertex4["y"].get<double>();
            vertex4Z = vertex4["z"].get<double>();

			verticesOBB.push_back(glm::vec3(vertex2X, vertex2Y, vertex2Z));
            verticesOBB.push_back(glm::vec3(vertex1X, vertex1Y, vertex1Z));
			verticesOBB.push_back(glm::vec3(vertex3X, vertex3Y, vertex3Z));
            verticesOBB.push_back(glm::vec3(vertex4X, vertex4Y, vertex4Z));


            //cogemos el plano1
            if(k==1){
                centerPlane1 = glm::vec3( (vertex1X+vertex2X+vertex3X+vertex4X)/4 , (vertex1Y+vertex2Y-vertex3Y-vertex4Y)/4 , (vertex1Z+vertex2Z+vertex3Z+vertex4Z)/4 );
                //cout << "el centro de plano1 es: ( " << centerPlane1.x << " , " << centerPlane1.y <<  " , " << centerPlane1.z << " )" << endl;
            }
            if(k==2){
                centerPlane2 = glm::vec3( (vertex1X+vertex2X+vertex3X+vertex4X)/4 , (vertex1Y+vertex2Y-vertex3Y-vertex4Y)/4 , (vertex1Z+vertex2Z+vertex3Z+vertex4Z)/4 );   
            }
            centerMass.x += vertex1X+vertex2X+vertex3X+vertex4X;
            centerMass.y += vertex1Y+vertex2Y+vertex3Y+vertex4Y;
            centerMass.z += vertex1Z+vertex2Z+vertex3Z+vertex4Z;
        }
        // aqui ya tenemos el array de OBB, nos faltan los centros de Massas 
        centerMass = glm::vec3( (centerMass.x/16) , (centerMass.y/16) , (centerMass.z/16) );
        //cout << "el centro de masas es: ( " << centerMass.x << " , " << centerMass.y <<  " , " << centerMass.z << " )" << endl;


        if( glm::distance(centerPlane1, centerMass) > glm::distance(centerPlane2, centerMass)){
            // el plano 1 por descarte es el pequeño
            auto aumentoZ = abs(verticesOBB[0].x - verticesOBB[2].x)/2;
            auto aumentoX = abs(verticesOBB[0].z - verticesOBB[2].z)/2;
            auto posibleCP1X = glm::vec3(centerPlane1.x+aumentoX, centerPlane1.y, centerPlane1.z);
            auto posibleCP2X = glm::vec3(centerPlane1.x-aumentoX, centerPlane1.y, centerPlane1.z);
            if(glm::distance(posibleCP1X, centerMass) > glm::distance(posibleCP2X, centerMass)){
                aumentoX = -aumentoX;
            }
            auto posibleCP1Z = glm::vec3(centerPlane1.x, centerPlane1.y, centerPlane1.z+aumentoZ);
            auto posibleCP2Z = glm::vec3(centerPlane1.x, centerPlane1.y, centerPlane1.z-aumentoZ);
            if(glm::distance(posibleCP1Z, centerMass) > glm::distance(posibleCP2Z, centerMass)){
                aumentoZ = -aumentoZ;
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
        centersMass.push_back(glm::vec3(posX, centerMass.y, posZ));
        centersMass.push_back(glm::vec3(posX2, centerMass.y, posZ2));

        CreateBoundingOBB(verticesOBB, centersMass );

    }

    //auto verticesOBB = new vector<glm::vec3>;
    //verticesOBB.push_back(glm::vec3(100.f, -100.f, 50.f));
    //verticesOBB.push_back(glm::vec3(100.f, -100.f, 150.f));
    //verticesOBB.push_back(glm::vec3(100.f, 100.f, 150.f));
    //verticesOBB.push_back(glm::vec3(100.f, 100.f, 50.f));

    //verticesOBB.push_back(glm::vec3(100.f, 100.f, 50.f));
    //verticesOBB.push_back(glm::vec3(150.f, 100.f, 50.f));
    //verticesOBB.push_back(glm::vec3(150.f, -100.f, 50.f));
    //verticesOBB.push_back(glm::vec3(100.f, -100.f, 50.f));

    //verticesOBB.push_back(glm::vec3(100.f, -100.f, 150.f));
    //verticesOBB.push_back(glm::vec3(150.f, -100.f, 150.f));
    //verticesOBB.push_back(glm::vec3(150.f, 100.f, 150.f));
    //verticesOBB.push_back(glm::vec3(100.f, 100.f, 150.f));

    //verticesOBB.push_back(glm::vec3(150.f, 100.f, 50.f));
    //verticesOBB.push_back(glm::vec3(150.f, 100.f, 150.f));
    //verticesOBB.push_back(glm::vec3(150.f, -100.f, 150.f));
    //verticesOBB.push_back(glm::vec3(150.f, -100.f, 50.f));

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