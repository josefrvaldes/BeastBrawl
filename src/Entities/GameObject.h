#ifndef __GameObject_H__
#define __GameObject_H__

#include "Entity.h"
#include <iostream>
#include <string>
using namespace std;

class GameObject : public Entity
{

public:
    GameObject();
    GameObject(float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float scaX, float scaY, float scaZ, string texture, string mesh, float maxSpeed, float acceleration, float carFriction, float carSlowDown);
    ~GameObject();
};

#endif