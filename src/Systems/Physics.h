#pragma once


#include "../EventManager/Event.h"
#include "../EventManager/EventManager.h"
#include "include/glm/vec3.hpp"
#include "include/glm/geometric.hpp"

#include <math.h>
#include <memory>

using namespace std;
using namespace glm;
#define PI 3.141592

class Car;
class Camera;
class CCar;
class CTransformable;
class CExternalForce;
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
    void CalculatePosition(CCar *cCar, CTransformable *cTransformable, CSpeed *cSpeed, CExternalForce *cExternalForce, float deltaTime);
    void CalculatePositionReverse(CCar *cCar, CTransformable *cTransformable, CExternalForce *cExternalForce, float deltaTime);
    void CalculatePositionCamera(CTransformable *cTransformableCar, CTransformable *cTransformableCamera, CCamera *cCamera);
    glm::vec3 CalculateVecDirCar(CTransformable *cTransformable) const;
    void FrictionExternalForce(CCar *cCar, CExternalForce *externalForce) const;
    float deltaTime;
};