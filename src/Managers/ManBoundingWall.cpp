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
 
 
 #include "ManBoundingWall.h"

#include <iostream>
#include <Entities/BoundingWall.h>
#include <include_json/include_json.hpp>
#include <fstream>
#include <cstring>
#include <sstream>

using namespace std;
using json = nlohmann::json;
ManBoundingWall::ManBoundingWall() {
    SubscribeToEvents();
    // el primer bool marca la orientación: si es true está orientado en el eje X, si es false, está orientado en el eje Z
    // el segundo bool marca el sentido del vector normal
    //shared_ptr<BoundingWall> p1 = make_shared<BoundingWall>(vec3(-500.f, 20.f, -460.f), true, false);
    //shared_ptr<BoundingWall> p2 = make_shared<BoundingWall>(vec3(-500.f, 20.f, 760.f), true, true); // orientation
    //shared_ptr<BoundingWall> p3 = make_shared<BoundingWall>(vec3(-470.f, 20.f, -480.f), false, true);
    //shared_ptr<BoundingWall> p4 = make_shared<BoundingWall>(vec3(480.f, 20.f, -480.f), false, false);
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

    long unsigned int navMeshCount = j["BOUNDING PLANES"].size();
    //std::cout << "EL NUMERO DE Bounding Planes EN EL JSON ES: " << navMeshCount << std::endl;
    //Leemos el array de NavMesh
    for(long unsigned int i = 0; i< navMeshCount; ++i){
        string idNavMesh = to_string(i);  //El primer elemento es "NAVMESH 1"
        auto boundingPlaneActual = j["BOUNDING PLANES"][idNavMesh];
        // cogemos los 4 vertices
        auto vertex1 = boundingPlaneActual["vertex1"];
        vertex1X = vertex1["x"].get<double>();
        vertex1Y = vertex1["y"].get<double>();
        vertex1Z = vertex1["z"].get<double>();
        vec3 v_vertex1 = vec3(vertex1X, vertex1Y, vertex1Z);
        auto vertex2 = boundingPlaneActual["vertex2"];
        vertex2X = vertex2["x"].get<double>();
        vertex2Y = vertex2["y"].get<double>();
        vertex2Z = vertex2["z"].get<double>();
        vec3 v_vertex2 = vec3(vertex2X, vertex2Y, vertex2Z);
        auto vertex3 = boundingPlaneActual["vertex3"];
        vertex3X = vertex3["x"].get<double>();
        vertex3Y = vertex3["y"].get<double>();
        vertex3Z = vertex3["z"].get<double>();
        vec3 v_vertex3 = vec3(vertex3X, vertex3Y, vertex3Z);
        auto vertex4 = boundingPlaneActual["vertex4"];
        vertex4X = vertex4["x"].get<double>();
        vertex4Y = vertex4["y"].get<double>();
        vertex4Z = vertex4["z"].get<double>();
        vec3 v_vertex4 = vec3(vertex4X, vertex4Y, vertex4Z);

        CreateBoundingWall(v_vertex2, v_vertex1, v_vertex3, v_vertex4);
    }

    //CreateBoundingWall(vec3(-500.f, 20.f, -460.f),vec3(550.f, 20.f, -460.f),vec3(550.f, 120.f, -460.f),vec3(-500.f, 120.f, -460.f));
    //CreateBoundingWall(vec3(-500.f, 120.f, 760.f),vec3(550.f, 120.f, 760.f),vec3(550.f, 20.f, 760.f),vec3(-500.f, 20.f, 760.f));
    //CreateBoundingWall(vec3(-470.f, 120.f, -480.f),vec3(-470.f, 120.f, 820.f),vec3(-470.f, 20.f, 820.f),vec3(-470.f, 20.f, -480.f));
    //CreateBoundingWall(vec3(480.f, 20.f, -480.f),vec3(480.f, 20.f, 820.f),vec3(480.f, 120.f, 820.f),vec3(480.f, 120.f, -480.f));


    // 4 Planos normales
    //CreateBoundingWall(vec3(50.f, -40.f, 50.f),vec3(150.f, -40.f, 50.f),vec3(150.f, -300.f, 50.f),vec3(50.f, -300.f, 50.f));
    //CreateBoundingWall(vec3(50.f, -300.f, 150.f),vec3 (150.f, -300.f, 150.f),vec3(150.f, -40.f, 150.f),vec3(50.f, -40.f, 150.f));
    //CreateBoundingWall(vec3(50.f, -300.f, 50.f),vec3(50.f, -300.f, 150.f),vec3(50.f, -40.f, 150.f),vec3(50.f, -40.f, 50.f));
    //CreateBoundingWall(vec3(150.f, -40.f, 50.f),vec3(150.f, -40.f, 150.f),vec3(150.f, -300.f, 150.f),vec3(150.f, -300.f, 50.f));



    // con 4 planos creamos un mini Cubo para cmporbar colisiones
    //CreateBoundingWall(vec3(40.f, 40.f, 40.f),vec3(140.f, 40.f, 40.f),vec3(140.f, 10.f, 40.f),vec3(40.f, 10.f, 40.f));
    //CreateBoundingWall(vec3(40.f, 10.f, 160.f),vec3 (140.f, 10.f, 160.f),vec3(140.f, 40.f, 160.f),vec3(40.f, 40.f, 160.f));
    //CreateBoundingWall(vec3(30.f, 10.f, 50.f),vec3(30.f, 10.f, 150.f),vec3(30.f, 40.f, 150.f),vec3(30.f, 40.f, 50.f));
    //CreateBoundingWall(vec3(150.f, 40.f, 50.f),vec3(150.f, 40.f, 150.f),vec3(150.f, 10.f, 150.f),vec3(150.f, 10.f, 50.f));
//
    //CreateBoundingWall(vec3(30.f, 40.f, 50.f),vec3(40.f, 40.f, 40.f),vec3(40.f, 10.f, 40.f),vec3(30.f, 10.f, 50.f));


    // Por consistencia para crear un plano


    //cout << "Hemos creado un bounding wall, ahora tenemos " << entities.size() << " bounding walls" << endl;
}

void ManBoundingWall::CreateBoundingWall(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d) {
    shared_ptr<BoundingWall> p = make_shared<BoundingWall>(a, b, c, d);
    entities.push_back(p);
}

ManBoundingWall::~ManBoundingWall() {
}

void ManBoundingWall::SubscribeToEvents() {
    // EventManager::GetInstance()->SuscribeMulti(Listener(
    //     EventType::PRESS_I,
    //     bind(&ManCar::AccelerateCar, this, placeholders::_1),
    //     "AccelerateCar"));
}

void ManBoundingWall::Integrate(float delta) {
    //physics->update(GetCar().get(), cam.get());
}