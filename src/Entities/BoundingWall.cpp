#include "BoundingWall.h"
#include <iostream>
#include "../Components/CBoundingPlane.h"
#include "../Components/CId.h"

class Position;
using namespace std;

BoundingWall::BoundingWall() {
    shared_ptr<CId> cId = make_shared<CId>();
    // TODO: estos datos son de pegote, cambiar
    vec3 pos(-20.f, 20.f, -300.f);
    vec3 a(pos.x, pos.y + 20.f, pos.z);
    vec3 b(pos.x + 20.f, pos.y + 20.f, pos.z);
    vec3 c(pos.x + 20.f, pos.y, pos.z);
    vec3 d(pos.x, pos.y, pos.z);
    shared_ptr<CBoundingPlane> cPlane = make_shared<CBoundingPlane>(a, b, c, d);
    AddComponent(cId);
    AddComponent(cPlane);
}

BoundingWall::BoundingWall(const vec3 &a_, const vec3 &b_, const vec3 &c_, const vec3 &d_) {
    shared_ptr<CId> cId = make_shared<CId>();
    shared_ptr<CBoundingPlane> cPlane = make_shared<CBoundingPlane>(a_, b_, c_, d_);
    AddComponent(cId);
    AddComponent(cPlane);
}

BoundingWall::~BoundingWall() {
}
