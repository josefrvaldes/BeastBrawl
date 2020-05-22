/**
 * Beast Brawl
 * Game created as a final project of the Multimedia Engineering Degree in the University of Alicante.
 * Made by Clover Games Studio, with members 
 * Carlos de la Fuente Torres delafuentetorresc@gmail.com,
 * Antonio Jose Martinez Garcia https://www.linkedin.com/in/antonio-jose-martinez-garcia/,
 * Jesús Mas Carretero jmasc03@gmail.com, 
 * Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/, 
 * Rubén Rubio Martínez https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/, 
 * and Jose Valdés Sirvent https://www.linkedin.com/in/jose-f-valdés-sirvent-6058b5a5/ github -> josefrvaldes
 * 
 * 
 * @author Antonio Jose Martinez Garcia
 * 
 */
 
 
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
class CBufferOnline;
class COnline;
class CCamera;
class CSpeed;
class CNitro;

class Physics {
   public:
    Physics(float);
    ~Physics(){};
    void update(Car* car);
    void Accelerate(Car *);
    void TurnLeft(Car *);
    void TurnRight(Car *);
    void NotAcceleratingOrDecelerating(Car *);
    void Decelerate(Car *);
    void NotTurning(Car *);
    void Skid(Car *);
    void NotSkidding(Car *);

    void UpdateHuman(Car* car, int64_t gameTime);
    void AccelerateHuman(CCar &cCar, CNitro &cNitro) const;
    void TurnLeftHuman(CCar &cCar) const;
    void TurnRightHuman(CCar &cCar) const;
    void NotAcceleratingOrDeceleratingHuman(CCar &cCar, CNitro &cNitro) const;
    void DecelerateHuman(CCar &cCar, CNitro &cNitro) const;
    void NotTurningHuman(CCar &cCar) const;
    void SkidHuman(CCar &cCar, CTransformable &cTrans) const;
    void NotSkiddingHuman(CCar &cCar, CTransformable &cTrans) const;
    void NewInputsReceivedOnline(Car *car, float speed, float wheelRotation, float skidDeg, float skidRotation, CBufferOnline *buffer);
    void NewSyncReceivedOnline(Car *car, int64_t timeSent, int64_t gameTime);

   protected:
   private:
    void MoveCarHumanByInput(Car *car, CCar *cCar, COnline *cOnline, CTransformable *cTransformable, CSpeed *cSpeed, CNitro *cNitro, CExternalForce *cExternalForce);
    void CalculatePosition(CCar *cCar, CTransformable *cTransformable, CSpeed *cSpeed, CExternalForce *cExternalForce, float deltaTime);
    void CalculatePositionReverse(CCar *cCar, CTransformable *cTransformable, CExternalForce *cExternalForce, float deltaTime);
    //void CalculatePositionCamera(CCar *cCar, CTransformable *cTransformableCar, CTransformable *cTransformableCamera, CCamera *cCamera);
    glm::vec3 CalculateVecDirCar(CTransformable *cTransformable) const;
    glm::vec2 ApplyExternalForce(CCar *cCar, CExternalForce *externalForce, const glm::vec2& carForce) const;

    void RecoverSkid(CCar &cCar, CTransformable &cTrans) const;

    float deltaTime;
};