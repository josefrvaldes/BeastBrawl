#pragma once

#include <EventManager/Event.h>
#include <EventManager/EventManager.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/geometric.hpp>

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
class CNitro;
class CSpeed;

class PhysicsCamera {
   public:
    PhysicsCamera(float);
    ~PhysicsCamera(){};
    void update(Car*, Camera*);
    void TurnLeft(Car *, Camera *);
    void TurnRight(Car *, Camera *);
    void NotTurning(Car *, Camera *);

   protected:
   private:
    float deltaTime;
    void CalculatePositionCamera(CCar *cCar, CTransformable *cTransformableCar, CTransformable *cTransformableCamera, CCamera *cCamera, CSpeed *cSpeed);
    void Accelerate(CCar *cCar, CCamera *cCamera);
    void Decelerate(CCar *cCar, CCamera *cCamera);
    void Accelerate_X(CCar *cCar, CCamera *cCamera, CSpeed *cSpeedCam);
    void Decelerate_X(CCar *cCar, CCamera *cCamera, CSpeed *cSpeedCam);
    void Accelerate_Z(CCar *cCar, CCamera *cCamera, CSpeed *cSpeedCam);
    void Decelerate_Z(CCar *cCar, CCamera *cCamera, CSpeed *cSpeedCam);
    void ApplyFrictionX(CCar *cCar, CCamera *cCamera, CSpeed *cSpeedCam);
    void ApplyFrictionZ(CCar *cCar, CCamera *cCamera, CSpeed *cSpeedCam);
    // general
    double Angle2Vectors(const glm::vec3 &a, const glm::vec3 &b) const;
};