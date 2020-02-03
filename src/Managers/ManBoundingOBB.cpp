#include "ManBoundingOBB.h"
#include <functional>
#include <iostream>
#include "../Entities/BoundingWall.h"
#include "../Entities/BoundingOBB.h"
#include "../../include/include_json/include_json.hpp"
#include <fstream>

using namespace std;
using json = nlohmann::json;

ManBoundingOBB::ManBoundingOBB() {
    SubscribeToEvents();



    // 4 Planos normales
    //ManBoundingOBB(vec3(50.f, 40.f, 50.f),vec3(150.f, 40.f, 50.f),vec3(150.f, 10.f, 50.f),vec3(50.f, 10.f, 50.f));
    //ManBoundingOBB(vec3(50.f, 10.f, 150.f),vec3 (150.f, 10.f, 150.f),vec3(150.f, 40.f, 150.f),vec3(50.f, 40.f, 150.f));
    //ManBoundingOBB(vec3(50.f, 10.f, 50.f),vec3(50.f, 10.f, 150.f),vec3(50.f, 40.f, 150.f),vec3(50.f, 40.f, 50.f));
    //ManBoundingOBB(vec3(150.f, 40.f, 50.f),vec3(150.f, 40.f, 150.f),vec3(150.f, 10.f, 150.f),vec3(150.f, 10.f, 50.f));

    auto verticesOBB = new vector<vec3>;
    verticesOBB->push_back(vec3(50.f, 40.f, 50.f));
    verticesOBB->push_back(vec3(150.f, 40.f, 50.f));
    verticesOBB->push_back(vec3(150.f, 10.f, 50.f));
    verticesOBB->push_back(vec3(50.f, 10.f, 50.f));

    verticesOBB->push_back(vec3(50.f, 10.f, 150.f));
    verticesOBB->push_back(vec3 (150.f, 10.f, 150.f));
    verticesOBB->push_back(vec3(150.f, 40.f, 150.f));
    verticesOBB->push_back(vec3(50.f, 40.f, 150.f));

    verticesOBB->push_back(vec3(50.f, 10.f, 50.f));
    verticesOBB->push_back(vec3(50.f, 10.f, 150.f));
    verticesOBB->push_back(vec3(50.f, 40.f, 150.f));
    verticesOBB->push_back(vec3(50.f, 40.f, 50.f));

    verticesOBB->push_back(vec3(150.f, 40.f, 50.f));
    verticesOBB->push_back(vec3(150.f, 40.f, 150.f));
    verticesOBB->push_back(vec3(150.f, 10.f, 150.f));
    verticesOBB->push_back(vec3(150.f, 10.f, 50.f));

    vec3 centerMass = vec3( (50+150+50+150+50+50+150+150)/8, (40+10+10+40+10+40+10+40)/8, (50+50+150+150+50+150+50+150)/8);

    // con 4 planos creamos un mini Cubo para cmporbar colisiones
    //CreateBoundingOBB(vec3(40.f, 40.f, 40.f),vec3(140.f, 40.f, 40.f),vec3(140.f, 10.f, 40.f),vec3(40.f, 10.f, 40.f));
    //CreateBoundingOBB(vec3(40.f, 10.f, 160.f),vec3 (140.f, 10.f, 160.f),vec3(140.f, 40.f, 160.f),vec3(40.f, 40.f, 160.f));
    //CreateBoundingOBB(vec3(30.f, 10.f, 50.f),vec3(30.f, 10.f, 150.f),vec3(30.f, 40.f, 150.f),vec3(30.f, 40.f, 50.f));
    //CreateBoundingOBB(vec3(150.f, 40.f, 50.f),vec3(150.f, 40.f, 150.f),vec3(150.f, 10.f, 150.f),vec3(150.f, 10.f, 50.f));
    //CreateBoundingOBB(vec3(30.f, 40.f, 50.f),vec3(40.f, 40.f, 40.f),vec3(40.f, 10.f, 40.f),vec3(30.f, 10.f, 50.f));

    //TODO: esta bien hacer ese new?????????

    //auto verticesOBB = new vector<vec3>;
    //verticesOBB->push_back(vec3(40.f, 40.f, 40.f));
    //verticesOBB->push_back(vec3(140.f, 40.f, 40.f));
    //verticesOBB->push_back(vec3(140.f, 10.f, 40.f));
    //verticesOBB->push_back(vec3(40.f, 10.f, 40.f));
//
    //verticesOBB->push_back(vec3(40.f, 10.f, 160.f));
    //verticesOBB->push_back(vec3 (140.f, 10.f, 160.f));
    //verticesOBB->push_back(vec3(140.f, 40.f, 160.f));
    //verticesOBB->push_back(vec3(40.f, 40.f, 160.f));
//
    //verticesOBB->push_back(vec3(30.f, 10.f, 50.f));
    //verticesOBB->push_back(vec3(30.f, 10.f, 150.f));
    //verticesOBB->push_back(vec3(30.f, 40.f, 150.f));
    //verticesOBB->push_back(vec3(30.f, 40.f, 50.f));
//
    //verticesOBB->push_back(vec3(150.f, 40.f, 50.f));
    //verticesOBB->push_back(vec3(150.f, 40.f, 150.f));
    //verticesOBB->push_back(vec3(150.f, 10.f, 150.f));
    //verticesOBB->push_back(vec3(150.f, 10.f, 50.f));
//
    //verticesOBB->push_back(vec3(30.f, 40.f, 50.f));
    //verticesOBB->push_back(vec3(40.f, 40.f, 40.f));
    //verticesOBB->push_back(vec3(40.f, 10.f, 40.f));
    //verticesOBB->push_back(vec3(30.f, 10.f, 50.f));
//
    //vec3 centerMass = vec3( (40+140+40+140+30+30+150+150+30+40)/10, (40+10+10+40+10+40+40+10+40+10)/10, (40+40+160+160+50+150+50+150+50+40)/10);

    CreateBoundingOBB(*verticesOBB, *(&centerMass) );




    // Por consistencia para crear un plano

}


void ManBoundingOBB::CreateBoundingOBB(const vector<vec3> &vertices, const vec3 &centerMas){
    shared_ptr<BoundingOBB> p = make_shared<BoundingOBB>(vertices, centerMas);
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