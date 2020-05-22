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
 * @author Clover Games Studio
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
class CCamera;
class CSpeed;
class CNitro;
class CSpeed;
class CHurt;

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
    void CalculatePositionCamera(CCar *cCar, CTransformable *cTransformableCar, CTransformable *cTransformableCamera, CCamera *cCamera, CSpeed *cSpeed, CHurt *cHurt);
    void CalculateOffsetCamera(const CCar &cCar, const CNitro &cNitro, CCamera *cCamera) const;
    float CalculateCameraDistance(const CCar &cCar, const CCamera &cCamera) const;

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