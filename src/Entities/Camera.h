#ifndef __CAMERA_H__
#define __CAMERA_H__
#include "Entity.h"
#include <iostream>

using namespace std;

class Camera : public Entity
{

public:
    Camera();
    Camera(float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float scaX, float scaY, float scaZ);
    ~Camera();
};


#endif
