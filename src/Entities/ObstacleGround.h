#pragma once
#include "Entity.h"
#include <iostream>

using namespace std;

class ObstacleGround : public Entity
{

public:
    ObstacleGround();
    ObstacleGround(float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float scaX, float scaY, float scaZ);
    ~ObstacleGround();
};
