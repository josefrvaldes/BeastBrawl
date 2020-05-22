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
