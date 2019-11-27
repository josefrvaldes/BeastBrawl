#ifndef __HERO_H__
#define __HERO_H__

#include "Entity.h"
#include <iostream>

using namespace std;

class Hero : public Entity
{

public:
    Hero();
    Hero(float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float scaX, float scaY, float scaZ);
    ~Hero();
};

#endif