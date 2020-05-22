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
 
 
 #include "BoundingOBB.h"
#include <iostream>
#include "../Components/CBoundingPlane.h"
#include "../Components/CBoundingOBB.h"
#include "../Components/CId.h"

class Position;
using namespace std;



//BoundingOBB::BoundingOBB() : BoundingBox(vec3(-20.f, 20.f, /*-460.f*/ 200.f)) {
//}


BoundingOBB::BoundingOBB(const vector<glm::vec3> &vertices, const vector<glm::vec3> &centersMass) {
    shared_ptr<CId> cId = make_shared<CId>();
    //shared_ptr<CBoundingPlane> cPlane = make_shared<CBoundingPlane>(a_, b_, c_, d_);
    shared_ptr<CBoundingOBB> cBox = make_shared<CBoundingOBB>(vertices, centersMass);
    AddComponent(cId);
    AddComponent(cBox);
}

BoundingOBB::~BoundingOBB() {
}
