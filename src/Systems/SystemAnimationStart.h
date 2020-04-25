#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include "../../include/glm/glm.hpp"
using namespace std;

class Entity;
class Camera;
class Totem;
class Car;



class SystemAnimationStart {
   public:
    SystemAnimationStart(Camera *cam_, Totem *totem_, Car *car_);
    void Animate();
    void ResetTimer();
    ~SystemAnimationStart() = default;

   private:
    Camera *cam;
    Totem *totem;
    Car *car;

    glm::vec3 currentStartPosition{50.f, 500.f, 50.f};
    glm::vec3 currentEndPosition{250.f, 500.f, 50.f};

    int64_t timeStartAnim;
    
};