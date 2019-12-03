#pragma once

#include "Entity.h"
#include <iostream>
#include <map>

using namespace std;

class WayPoint : public Entity{
public:
    WayPoint();
    ~WayPoint();
    WayPoint(float x, float y, float z);

    float getPosX(){ return posX;}
    float getPosY(){ return posY;}
    float getPosZ(){ return posZ;}   

private:
    float posX, posY, posZ;
};
