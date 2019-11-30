#ifndef __CAR_H__
#define __CAR_H__

#include "Entity.h"
#include <iostream>
#include <string>
using namespace std;

class Car : public Entity
{

public:
    Car();
    Car(float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float scaX, float scaY, float scaZ, string texture, string mesh, float maxSpeed, float acceleration, float carFriction, float carSlowDown);
    ~Car();



};

#endif