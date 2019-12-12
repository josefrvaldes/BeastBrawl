#ifndef __CAMERA_H__
#define __CAMERA_H__
#include "Entity.h"
#include "../../lib/glm/vec3.hpp"
#include <iostream>

using namespace std;

class Camera : public Entity
{

public:
    Camera();
    Camera(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);
    ~Camera();
};


#endif
