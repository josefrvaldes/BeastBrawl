#include "PhysicsCamera.h"
#include <Components/CCamera.h>
#include <Components/CCar.h>
#include <Components/CNitro.h>
#include <Components/CSpeed.h>
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
    auto cCamera = static_cast<CCamera *>(cam->GetComponent(CompType::CameraComp).get());
    auto cSpeedCam = static_cast<CSpeed *>(cam->GetComponent(CompType::SpeedComp).get());
    auto cTransformableCam = static_cast<CTransformable *>(cam->GetComponent(CompType::TransformableComp).get());

    CalculatePositionCamera(cCar, cTransformable, cTransformableCam, cCamera, cSpeedCam);
}

//Calcula la posicion de la camara (duda con las formulas preguntar a Jose)
void PhysicsCamera::CalculatePositionCamera(CCar *cCar, CTransformable *cTransformableCar, CTransformable *cTransformableCamera, CCamera *cCamera, CSpeed *cSpeedCam) {

    // comento la primera linea porque la pos de la cámara en altura (por ahora) es siempre la misma
    float rotationFinal = cTransformableCar->rotation.y - cCar->skidRotation - cCamera->rotExtraY;
    rotationFinal = Utils::GetAdjustedDegrees(rotationFinal);

    cTransformableCamera->position.y = cTransformableCar->position.y + 20;
    cTransformableCamera->position.z = (cTransformableCar->position.z - 40 * sin(((rotationFinal) * PI) / 180.0));
    cTransformableCamera->position.x = (cTransformableCar->position.x + 40 * cos(((rotationFinal) * PI) / 180.0));







    //rotFinal = Utils::GetAdjustedDegrees(rotFinal);
    //float rotFinal = cTransformableCar->rotation.y - cCar->skidRotation - cCamera->rotExtraY;
    //auto carPos =  cTransformableCar->position;
    //auto destPosition = glm::vec3((carPos.x + 40 * cos(((rotFinal)*PI)/180.0)), carPos.y + 20, (carPos.z - 40 * sin(((rotFinal)*PI)/180.0)));
//
//    //auto disDest_X = abs(destPosition.x - cTransformableCamera->position.x); 
//    //if( disDest_X < abs( cTransformableCar->position.x - cTransformableCamera->position.x))     Accelerate_X(cCar, cCamera, cSpeedCam);
//
//    //else                                                                                        Decelerate_X(cCar, cCamera, cSpeedCam);
//    //
//    //auto disDest_Z = abs(destPosition.z - cTransformableCamera->position.z); 
//    //if( disDest_Z < abs( cTransformableCar->position.z - cTransformableCamera->position.z))     Accelerate_Z(cCar, cCamera, cSpeedCam);
//    else                                                                                        Decelerate_Z(cCar, cCamera, cSpeedCam);
//    
//    double angleRotCamera = atan2(destPosition.z-cTransformableCamera->position.z, destPosition.x-cTransformableCamera->position.x)*180/M_PI;
//    angleRotCamera = 180.0 - angleRotCamera; // se le restan ya que el eje empieza en el lado contrario 
//    if(angleRotCamera<0)
//        angleRotCamera += 360;

//    auto speedX = -cos(radians(angleRotCamera))*cSpeedCam->speed.x;  
//    auto speedZ = sin(radians(angleRotCamera))*cSpeedCam->speed.z;  
//
//    //cout << "el cCamara speed X es: " << cSpeedCam->speed.x <<" el speedX es: " << speedX << " y por el delta es: " << speedX * deltaTime << endl;
//    if( abs(speedX * deltaTime) < disDest_X)  cTransformableCamera->position.x += speedX * deltaTime;
//
//    else{
//        cTransformableCamera->position.x = destPosition.x;
//        cSpeedCam->speed.x = cCar->speed;
//    }                                    
//    if( abs(speedZ * deltaTime) < disDest_Z)  cTransformableCamera->position.z += speedZ * deltaTime;
//    else{
//        cTransformableCamera->position.z = destPosition.z;
//        cSpeedCam->speed.z = cCar->speed;
//    }   
//    cTransformableCamera->position.y  = destPosition.y;

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


void PhysicsCamera::Accelerate(Car *, Camera *){
}
void PhysicsCamera::Decelerate(Car *, Camera *){
    
}
void PhysicsCamera::NotAcceleratingOrDecelerating(Car *, Camera *){
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

        if (cCamera->rotExtraY >= cCar->decrementWheelRotation) {
            cCamera->rotExtraY -= cCamera->decrementrotExtraY;
        } else if (cCamera->rotExtraY <= -cCar->decrementWheelRotation) {
            cCamera->rotExtraY += cCamera->decrementrotExtraY;
        } else {
            cCamera->rotExtraY = 0;
        }
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
        if (cCamera->rotExtraY >= cCar->decrementWheelRotation) {
            cCamera->rotExtraY -= cCamera->decrementrotExtraY;
        } else if (cCamera->rotExtraY <= -cCar->decrementWheelRotation) {
            cCamera->rotExtraY += cCamera->decrementrotExtraY;
        } else {
            cCamera->rotExtraY = 0;
        }
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