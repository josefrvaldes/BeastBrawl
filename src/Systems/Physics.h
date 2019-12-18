#pragma once

#include "../Components/CCamera.h"
#include "../Components/CCar.h"
#include "../Components/CId.h"
#include "../Components/CTransformable.h"
#include "../Components/CType.h"
#include "../Components/CNitro.h"
#include "../Components/CShield.h"
#include "../Components/CRoboJorobo.h"
#include "../Components/Component.h"

#include "../EventManager/Event.h"
#include "../EventManager/EventManager.h"


#include <math.h>
#include <memory>

using namespace std;
#define PI 3.141592

class Car;
class Camera;

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
    void CalculatePosition(CCar *cCar, CTransformable *cTransformable, float deltaTime);
    void CalculatePositionReverse(CCar *cCar, CTransformable *cTransformable, float deltaTime);
    void CalculatePositionCamera(CTransformable *cTransformableCar, CTransformable *cTransformableCamera, CCamera *cCamera);
    float deltaTime;
};