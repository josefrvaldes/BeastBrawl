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



class SystemAnimationEnd {
   public:
    SystemAnimationEnd(Camera *cam_);
    void Animate();
    void ResetTimer();
    ~SystemAnimationEnd() = default;
    void SetWinner(Car* winner_);
    Car* GetWinner() {return winner;};

   private:
    void MoveCam(int64_t interval, int64_t duration);

    Camera *cam;
    Car *winner;
    
    float rotation {45};
    const uint8_t ROTATION_SPEED = 1;

    int64_t timeStartAnim;
};