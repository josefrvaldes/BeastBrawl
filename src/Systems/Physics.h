#pragma once


#include <EventManager/Event.h>
#include <EventManager/EventManager.h>
#include <glm/vec3.hpp>
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
    void Skid(Car *, Camera *);
    void NotSkidding(Car *, Camera *);

    void UpdateHuman(Car* car);
    void AccelerateHuman(CCar &cCar, CNitro &cNitro) const;
    void TurnLeftHuman(CCar &cCar) const;
    void TurnRightHuman(CCar &cCar) const;
    void NotAcceleratingOrDeceleratingHuman(CCar &cCar, CNitro &cNitro) const;
    void DecelerateHuman(CCar &cCar, CNitro &cNitro) const;
    void NotTurningHuman(CCar &cCar) const;
    void SkidHuman(CCar &cCar, CTransformable &cTrans) const;
    void NotSkiddingHuman(CCar &cCar, CTransformable &cTrans) const;

   protected:
   private:
    void CalculatePosition(CCar *cCar, CTransformable *cTransformable, CSpeed *cSpeed, CExternalForce *cExternalForce, float deltaTime);
    void CalculatePositionReverse(CCar *cCar, CTransformable *cTransformable, CExternalForce *cExternalForce, float deltaTime);
    void CalculatePositionCamera(CCar *cCar, CTransformable *cTransformableCar, CTransformable *cTransformableCamera, CCamera *cCamera);
    glm::vec3 CalculateVecDirCar(CTransformable *cTransformable) const;
    void FrictionExternalForce(CCar *cCar, CExternalForce *externalForce) const;

    void RecoverSkid(CCar &cCar, CTransformable &cTrans) const;

    float deltaTime;
};