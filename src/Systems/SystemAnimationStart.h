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
    SystemAnimationStart(Camera *cam_, Totem *totem_, Car* mainCar_, Car *car_);
    bool Animate();
    void ResetTimer();
    ~SystemAnimationStart() = default;

   private:
    void MoveCam(int64_t interval, int64_t duration);
    void AnimatePanMap();
    void CalculatePosStartEndTotem();
    void AnimateTotem();
    void CalculatePosStartEndRandomCar();
    void AnimateRandomCar();
    void CalculatePosStartEndMainCar();
    void CalculatePosStartEndCar(Car *car);
    void AnimateMainCar();

    Camera *cam;
    Totem *totem;
    Car *mainCar;
    Car *car;

    glm::vec3 currentStartPosition{50.f, 500.f, 50.f};
    glm::vec3 currentEndPosition{250.f, 500.f, 50.f};

    std::vector<int64_t> durations{3000,3000,3000,3000};
    size_t currentIndexAnimation {0};

    int64_t timeStartAnim;
    
};