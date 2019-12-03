#pragma once

#include "Entity.h"
#include <iostream>
#include <map>

#include "../../lib/glm/vec3.hpp"

using namespace std;

class WayPoint : public Entity{
public:
    WayPoint();
    ~WayPoint();
    WayPoint(glm::vec3 _position);

    float getPosX(){ return posX;}
    float getPosY(){ return posY;}
    float getPosZ(){ return posZ;}   

private:
    float posX, posY, posZ;
};
