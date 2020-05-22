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
 
 
 #include "PhysicsCamera.h"
#include <Components/CCamera.h>
#include <Components/CCar.h>
#include <Components/CNitro.h>
#include <Components/CSpeed.h>
#include <Components/CHurt.h>
#include <Entities/Camera.h>
#include <Entities/Car.h>
#include "../Entities/CarHuman.h"
#include "../Components/COnline.h"
#include "../Components/CExternalForce.h"
#include "../Systems/Utils.h"
#include "../../include/glm/glm.hpp"

#include <cmath>

#include <iostream>
#include <math.h>

#include "../Constants.h"

PhysicsCamera::PhysicsCamera(float _deltaTime) : deltaTime(_deltaTime) {
}

void PhysicsCamera::update(Car *car, Camera *cam) {
    // actualizar posiciones
    auto cTransformable = static_cast<CTransformable *>(car->GetComponent(CompType::TransformableComp).get());
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());
    auto cNitro = static_cast<CNitro *>(car->GetComponent(CompType::NitroComp).get());
    auto cCamera = static_cast<CCamera *>(cam->GetComponent(CompType::CameraComp).get());
    auto cSpeedCam = static_cast<CSpeed *>(cam->GetComponent(CompType::SpeedComp).get());
    auto cTransformableCam = static_cast<CTransformable *>(cam->GetComponent(CompType::TransformableComp).get());
    auto cHurt = static_cast<CHurt *>(car->GetComponent(CompType::HurtComp).get());

    CalculateOffsetCamera(*cCar, *cNitro, cCamera);
    CalculatePositionCamera(cCar, cTransformable, cTransformableCam, cCamera, cSpeedCam, cHurt);
}


void PhysicsCamera::CalculateOffsetCamera(const CCar &cCar, const CNitro &cNitro, CCamera *cCamera) const{
    if(!cNitro.activePowerUp){
        if(cCamera->actualDistance > cCamera->perfectDistance){         // recuperacion en caso de haber acabado la aceleracion con el nitro -> acercar
            cCamera->actualDistance -= cCamera->camDeceleration;
            if(cCamera->actualDistance < cCamera->perfectDistance)
                cCamera->actualDistance = cCamera->perfectDistance;
        }else if(cCar.speed < cCar.maxSpeed*0.3){                       // en caso de ir lento o chocarse con una pared -> acercar
            float distanceNow = CalculateCameraDistance(cCar, *cCamera);
            cCamera->actualDistance = distanceNow;
        }else if(cCamera->actualDistance<cCamera->perfectDistance){     // recuperar cuando aceleras -> alejar
            cCamera->actualDistance += cCamera->camAceleration;
            if(cCamera->actualDistance > cCamera->maxDistance)
                cCamera->actualDistance = cCamera->maxDistance;  
        }                         
    }else{                                                              // aceleracion con el nitro -> alejar
        cCamera->actualDistance += cCamera->camAceleration;
        if(cCamera->actualDistance > cCamera->maxDistance)
            cCamera->actualDistance = cCamera->maxDistance;
    }
}

// se calcula el valor de proximidad de la camara
float PhysicsCamera::CalculateCameraDistance(const CCar &cCar, const CCamera &cCamera) const{
    float distance = cCamera.perfectDistance - cCamera.minDistance;
    if(cCar.speed>0){
        distance = distance * (cCar.speed/(cCar.maxSpeed*0.3)) + cCamera.minDistance;
        if(distance>cCamera.perfectDistance)
            distance = cCamera.perfectDistance;
    }else{
        distance = cCamera.minDistance;
    }
    
    return distance;
}



//Calcula la posicion de la camara (duda con las formulas preguntar a Jose)
void PhysicsCamera::CalculatePositionCamera(CCar *cCar, CTransformable *cTransformableCar, CTransformable *cTransCam, CCamera *cCamera, CSpeed *cSpeedCam, CHurt *cHurt) {
    float rotationFinal = cTransformableCar->rotation.y - cCar->skidRotation - cCamera->rotExtraY - cHurt->currentRotation;
    rotationFinal = Utils::GetAdjustedDegrees(rotationFinal);
    

    cTransCam->position.y = cTransformableCar->position.y + cCamera->perfectUpDistance;
    cTransCam->position.z = cTransformableCar->position.z - (cCamera->actualDistance-cCamera->collisionDistance) * sin(glm::radians(rotationFinal));
    cTransCam->position.x = cTransformableCar->position.x + (cCamera->actualDistance-cCamera->collisionDistance) * cos(glm::radians(rotationFinal));

    //float rotFinal = cTransformableCar->rotation.y - cCar->skidRotation - cCamera->rotExtraY;
    //auto carPos =  cTransformableCar->position;
    //auto destPosition = glm::vec3((carPos.x + 40 * cos(((rotFinal)*PI)/180.0)), carPos.y + 20, (carPos.z - 40 * sin(((rotFinal)*PI)/180.0)));
    //auto maxPosition = glm::vec3((carPos.x + 50 * cos(((rotFinal)*PI)/180.0)), carPos.y + 20, (carPos.z - 50 * sin(((rotFinal)*PI)/180.0)));
    //auto disDest_X = abs(destPosition.x - cTransCam->position.x); 
    //if( disDest_X < abs( cTransformableCar->position.x - cTransCam->position.x))     Accelerate_X(cCar, cCamera, cSpeedCam);
    //else                                                                                        Decelerate_X(cCar, cCamera, cSpeedCam);
    //
    //auto disDest_Z = abs(destPosition.z - cTransCam->position.z); 
    //if( disDest_Z < abs( cTransformableCar->position.z - cTransCam->position.z))     Accelerate_Z(cCar, cCamera, cSpeedCam);
    //else                                                                                        Decelerate_Z(cCar, cCamera, cSpeedCam);
    //
//
//
    //double angleRotCamera = atan2(destPosition.z-cTransCam->position.z, destPosition.x-cTransCam->position.x)*180/M_PI;
    //angleRotCamera = 180.0 - angleRotCamera; // se le restan ya que el eje empieza en el lado contrario 
    //if(angleRotCamera<0)
    //    angleRotCamera += 360;
//
    //auto speedX = -cos(radians(angleRotCamera))*cSpeedCam->speed.x;  
    //auto speedZ = sin(radians(angleRotCamera))*cSpeedCam->speed.z;  
//
    ////cout << "el cCamara speed X es: " << cSpeedCam->speed.x <<" el speedX es: " << speedX << " y por el delta es: " << speedX * deltaTime << endl;
    //if( abs(speedX * deltaTime) < disDest_X)  cTransCam->position.x += speedX * deltaTime;
    //else{
    //    cTransCam->position.x = destPosition.x;
    //    cSpeedCam->speed.x = cCar->speed;
    //}          
//
    //if( abs(speedZ * deltaTime) < disDest_Z)  cTransCam->position.z += speedZ * deltaTime;
    //else{
    //    cTransCam->position.z = destPosition.z;
    //    cSpeedCam->speed.z = cCar->speed;
    //}   
    //cTransCam->position.y  = destPosition.y;
//
//
    //if( disDest_X > abs(destPosition.x - maxPosition.x)){     
    //    cTransCam->position.x = maxPosition.x; 
    //}
    //if( disDest_Z > abs(destPosition.z - maxPosition.z)){     
    //    cTransCam->position.z = maxPosition.z; 
    //}
//
//
    //if( abs(cTransCam->position.x - maxPosition.x) > abs(destPosition.x - maxPosition.x)){     
    //    cTransCam->position.x = destPosition.x; 
    //}
    //if( abs(cTransCam->position.z - maxPosition.z) > abs(destPosition.z - maxPosition.z)){     
    //    cTransCam->position.z = destPosition.z; 
    //}


}



//Entra cuando se presionaS la A
void PhysicsCamera::TurnLeft(Car *car, Camera *cam) {
    //Componentes de la camara
    auto cCamera = static_cast<CCamera*>(cam->GetComponent(CompType::CameraComp).get());
    //Componentes del coche
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());

    if (cCar->speed >= cCar->maxSpeed*0.15) {
        if (cCamera->rotExtraY > -(cCar->maxWheelRotation + cCamera->rotExtraCamera)) {
            cCamera->rotExtraY -= cCamera->incrementrotExtraY;
        }
    } else if (cCar->speed <= -cCar->maxSpeed*0.15) {
        if (cCamera->rotExtraY > -(cCar->maxWheelRotation + cCamera->rotExtraCamera)) {
            cCamera->rotExtraY -= cCamera->incrementrotExtraY;
        }
    } else {  // la rueda vuelve a su sitio original al no dejarte rotar


    }
}

//Entra cuando se presiona la D
void PhysicsCamera::TurnRight(Car *car, Camera *cam) {
    //Componentes de la camara
    auto cCamera = static_cast<CCamera *>(cam->GetComponent(CompType::CameraComp).get());

    //Componentes del coche
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());

    if (cCar->speed >= cCar->maxSpeed*0.15) {
        if (cCamera->rotExtraY < (cCar->maxWheelRotation + cCamera->rotExtraCamera)) {
            cCamera->rotExtraY += cCamera->incrementrotExtraY;
        }
    } else if (cCar->speed <= -cCar->maxSpeed*0.15) {
        if (cCamera->rotExtraY < (cCar->maxWheelRotation + cCamera->rotExtraCamera)) {
            cCamera->rotExtraY += cCamera->incrementrotExtraY;
        }
    } else {  // la rueda vuelve a su sitio original al no dejarte rotar

    }
}

//Aqui entra cuando no se esta presionando ni A ni D
void PhysicsCamera::NotTurning(Car *car, Camera *cam) {
    //Componentes de la camara
    auto cCamera = static_cast<CCamera *>(cam->GetComponent(CompType::CameraComp).get());
    //Componentes del coche
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());

    if (cCamera->rotExtraY >= cCar->decrementWheelRotation) {
        cCamera->rotExtraY -= cCamera->decrementrotExtraY;
    } else if (cCamera->rotExtraY <= -cCar->decrementWheelRotation) {
        cCamera->rotExtraY += cCamera->decrementrotExtraY;
    } else {
        cCamera->rotExtraY = 0;
    }
}








void PhysicsCamera::Accelerate_X(CCar *cCar, CCamera *cCamera, CSpeed *cSpeedCam){
    cSpeedCam->speed.x += cCamera->acceleration;
    if(cSpeedCam->speed.x > cCamera->maxSpeed) cSpeedCam->speed.x = cCamera->maxSpeed;
}
void PhysicsCamera::Decelerate_X(CCar *cCar, CCamera *cCamera, CSpeed *cSpeedCam){
    cSpeedCam->speed.x -= cCamera->slowDown;
    if (cSpeedCam->speed.x < cCamera->reverseMaxSpeed) cSpeedCam->speed.x = cCamera->reverseMaxSpeed;
}

void PhysicsCamera::Accelerate_Z(CCar *cCar, CCamera *cCamera, CSpeed *cSpeedCam){
    cSpeedCam->speed.z += cCamera->acceleration;
    if(cSpeedCam->speed.z > cCamera->maxSpeed) cSpeedCam->speed.z = cCamera->maxSpeed;
}
void PhysicsCamera::Decelerate_Z(CCar *cCar, CCamera *cCamera, CSpeed *cSpeedCam){
    cSpeedCam->speed.z -= cCamera->slowDown;
    if (cSpeedCam->speed.z < cCamera->reverseMaxSpeed) cSpeedCam->speed.z = cCamera->reverseMaxSpeed;
}


void PhysicsCamera::Accelerate(CCar *cCar, CCamera *cCamera){
    cCamera->speed += cCamera->acceleration;
    if(cCamera->speed > cCamera->maxSpeed) cCamera->speed = cCamera->maxSpeed;
}
void PhysicsCamera::Decelerate(CCar *cCar, CCamera *cCamera){
    cCamera->speed -= cCamera->slowDown;
    if (cCamera->speed < cCamera->reverseMaxSpeed) cCamera->speed = cCamera->reverseMaxSpeed;
}
























void PhysicsCamera::ApplyFrictionX(CCar *cCar, CCamera *cCamera, CSpeed *cSpeedCam){
    if(cSpeedCam->speed.x > cCar->speed){
        cSpeedCam->speed.x -= cCamera->slowDown;
    }
    if(cSpeedCam->speed.x < cCar->speed){
        cSpeedCam->speed.x += cCamera->acceleration;
    }
}
void PhysicsCamera::ApplyFrictionZ(CCar *cCar, CCamera *cCamera, CSpeed *cSpeedCam){
    if(cSpeedCam->speed.z > cCar->speed){
        cSpeedCam->speed.z -= cCamera->slowDown;
    }
    if(cSpeedCam->speed.z < cCar->speed){
        cSpeedCam->speed.z += cCamera->acceleration;
    }  
}

double PhysicsCamera::Angle2Vectors(const vec3 &a, const vec3 &b) const {
    vec3 aN = glm::normalize(a);
    vec3 bN = glm::normalize(b);

    double dot = glm::dot(aN, bN);
    // Force the dot product of the two input vectors to
    // fall within the domain for inverse cosine, which
    // is -1 <= x <= 1. This will prevent runtime
    // "domain error" math exceptions.
    dot = (dot < -1.0 ? -1.0 : (dot > 1.0 ? 1.0 : dot));

    double angleRad = acos(dot);
    // grados = radianes*(180/PI_)
    return angleRad;
}


    //////////////////////// debugeando  /////////////////////////////////////////////
    //auto carT = cTransformableCar->position;
    //auto camT = cTransformableCamera->position;
    //cout << "el coche esta en   : (" << carT.x << " , " << carT.y << " , " << carT.z << " )" << endl; 
    //cout << "la camara esta en  : (" << camT.x << " , " << camT.y << " , " << camT.z << " )" << endl; 
    /////////////////////////////////////////////////////////////////////////////////

/* Basurilla

    bool frictionX = true;
    bool frictionZ = true;
    // comento la primera linea porque la pos de la cámara en altura (por ahora) es siempre la misma
    float rotationFinal = cTransformableCar->rotation.y - cCar->skidRotation - cCamera->rotExtraY;
    rotationFinal = Utils::GetAdjustedDegrees(rotationFinal);


    //////////////////////// debugeando  /////////////////////////////////////////////
    auto carT = cTransformableCar->position;
    auto camT = cTransformableCamera->position;
    cout << "el coche esta en   : (" << carT.x << " , " << carT.y << " , " << carT.z << " )" << endl; 
    cout << "la camara esta en  : (" << camT.x << " , " << camT.y << " , " << camT.z << " )" << endl; 
    /////////////////////////////////////////////////////////////////////////////////
    auto speedX = -cos(glm::radians(rotationFinal))*cSpeedCam->speed.x;  // * cCar->speed;
    auto speedZ = sin(glm::radians(rotationFinal))*cSpeedCam->speed.z;  // * cCar->speed;
    // MOVIMIENTO CAMARA _X_
    auto carPos_X = (cTransformableCar->position.x + 40 * cos(((rotationFinal) * PI) / 180.0));
    if(  cTransformableCamera->position.x  < (carPos_X - speedX * deltaTime)   ){
        // ACELERAR X
        //cout << "ACELERAMOS X" << endl;
        cSpeedCam->speed.x += cCamera->acceleration;
        if (cSpeedCam->speed.x > cCamera->maxSpeed) {
                cSpeedCam->speed.x = cCamera->maxSpeed;
        }
        frictionX = false;
    }
    else if(  cTransformableCamera->position.x  > (carPos_X + speedX * deltaTime)  ){
        // DESACELERAR X
        //cout << "                           DESACELERAMOS X" << endl;
        cSpeedCam->speed.x -= cCamera->slowDown;
        if (cSpeedCam->speed.x < cCamera->reverseMaxSpeed) {  // no se supere la velocidad marcha atras
            cSpeedCam->speed.x = cCamera->reverseMaxSpeed;
        }
        frictionX = false;
    }else{
        cout << "NADAAAA" << endl;
    }
    if(frictionX) ApplyFrictionX(cCar, cCamera, cSpeedCam);
    // MOVIMIENTO CAMARA _Z_
    //auto carPos_Z = (cTransformableCar->position.z - 40 * sin(((rotationFinal) * PI) / 180.0));
    //if(  cTransformableCamera->position.z  < (carPos_Z - speedZ * deltaTime)  ){
    //    // ACELERAR Z
    //    cSpeedCam->speed.z += cCamera->acceleration;
    //    if (cSpeedCam->speed.z > cCamera->maxSpeed) {
    //            cSpeedCam->speed.z = cCamera->maxSpeed;
    //    }
    //    frictionZ = false;
    //}
    //else if(  cTransformableCamera->position.z  > (carPos_Z + speedZ * deltaTime)  ){
    //    // DESACELERAR Z
    //    cSpeedCam->speed.z -= cCamera->slowDown;
    //    if (cSpeedCam->speed.z < cCamera->reverseMaxSpeed) {  // no se supere la velocidad marcha atras
    //        cSpeedCam->speed.z = cCamera->reverseMaxSpeed;
    //    }
    //    frictionZ = false;
    //}
    //if(frictionZ) ApplyFrictionZ(cCar, cCamera, cSpeedCam);
    // MOVIMIENTO CAMARA _Y_


    cTransformableCamera->position.y  = cTransformableCar->position.y + 20;
    cTransformableCamera->position.x += speedX * deltaTime;
    //cTransformableCamera->position.z += speedZ * deltaTime;



*/