#pragma once


#include "../EventManager/Event.h"
#include "../EventManager/EventManager.h"


#include <math.h>
#include <memory>

using namespace std;
#define PI 3.141592

class Car;
class Camera;
class CCar;
class CTransformable;
class CCamera;
class CSpeed;

class Physics {
   public:
    Physics(float);
    ~Physics(){};
    void update(Car* car, Camera* cam);
    void Accelerate(Car *, Camera *);
    void TurnLeft(Car *, Camera *);
    void TurnRight(Car *, Camera *);
    void NotAcceleratingOrDecelerating(Car *, Camera *);
    void Decelerate(Car *, Camera *);
    void NotTurning(Car *, Camera *);

   protected:
   private:
    void CalculatePosition(CCar *cCar, CTransformable *cTransformable, CSpeed *cSpeed, float deltaTime);
    void CalculatePositionReverse(CCar *cCar, CTransformable *cTransformable, float deltaTime);
    void CalculatePositionCamera(CTransformable *cTransformableCar, CTransformable *cTransformableCamera, CCamera *cCamera);
    float deltaTime;
};