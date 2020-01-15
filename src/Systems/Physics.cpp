#include "Physics.h"
#include <chrono>
#include "../Components/CCamera.h"
#include "../Components/CCar.h"
#include "../Components/CId.h"
#include "../Components/CNitro.h"
#include "../Components/CRoboJorobo.h"
#include "../Components/CShield.h"
#include "../Components/CSpeed.h"
#include "../Components/CTransformable.h"
#include "../Components/CType.h"
#include "../Components/Component.h"
#include "../Entities/Camera.h"
#include "../Entities/Car.h"

Physics::Physics(float _deltaTime) : deltaTime(_deltaTime) {
}

void Physics::update(Car *car, Camera *cam) {
    // actualizar posiciones
    auto cTransformable = static_cast<CTransformable *>(car->GetComponent(CompType::TransformableComp).get());
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());
    auto cCamera = static_cast<CCamera *>(cam->GetComponent(CompType::CameraComp).get());
    auto cTransformableCam = static_cast<CTransformable *>(cam->GetComponent(CompType::TransformableComp).get());
    auto cSpeed = static_cast<CSpeed *>(car->GetComponent(CompType::SpeedComp).get());
    if (cCar->speed >= 0)
        CalculatePosition(cCar, cTransformable, cSpeed, deltaTime);
    else
        CalculatePositionReverse(cCar, cTransformable, deltaTime);
    CalculatePositionCamera(cTransformable, cTransformableCam, cCamera);

}

//Calcula la posicion del coche (duda con las formulas preguntar a Jose)
void Physics::CalculatePosition(CCar *cCar, CTransformable *cTransformable, CSpeed *cSpeed, float deltaTime) {
    float angleRotation = (cTransformable->rotation.y * PI) / 180.0;
    //float delta = deltaTime;
    //Modificamos la posicion en X y Z en funcion del angulo

    cSpeed->speed.x = cos(angleRotation);  // * cCar->speed;
    cSpeed->speed.z = sin(angleRotation);  // * cCar->speed;
    cSpeed->speed.y = 0.f;                 // TODO, esto lo cacharreará el CLPhysics

    cTransformable->position.x -= cSpeed->speed.x * cCar->speed * deltaTime;
    cTransformable->position.z += cSpeed->speed.z * cCar->speed * deltaTime;

    //Si tiene rotacion, rotamos el coche
    // if (cCar->wheelRotation != 0) {
    cTransformable->rotation.y += cCar->wheelRotation * 0.20;
    if (cTransformable->rotation.y >= 360.0)
        cTransformable->rotation.y -= 360.0;
    else if (cTransformable->rotation.y < 0.0)
        cTransformable->rotation.y += 360.0;

    // }
}

//Calcula la posicion del coche (duda con las formulas preguntar a Jose)
void Physics::CalculatePositionReverse(CCar *cCar, CTransformable *cTransformable, float deltaTime) {
    float angleRotation = (cTransformable->rotation.y * PI) / 180.0;
    float delta = deltaTime;

    //Modificamos la posicion en X y Z en funcion del angulo
    cTransformable->position.z += sin(angleRotation) * cCar->speed * delta;
    cTransformable->position.x -= cos(angleRotation) * cCar->speed * delta;

    //Si tiene rotacion, rotamos el coche
    if (cCar->wheelRotation != 0) {
        cTransformable->rotation.y -= cCar->wheelRotation * 0.20;
        if (cTransformable->rotation.y >= 360.0)
            cTransformable->rotation.y -= 360.0;
        else if (cTransformable->rotation.y < 0.0)
            cTransformable->rotation.y += 360.0;
    }
}


//Calcula la posicion de la camara (duda con las formulas preguntar a Jose)
void Physics::CalculatePositionCamera(CTransformable *cTransformableCar, CTransformable *cTransformableCamera, CCamera *cCamera) {
    // comento la primera linea porque la pos de la cámara en altura (por ahora) es siempre la misma
    cTransformableCamera->position.y = cTransformableCar->position.y + 20;
    cTransformableCamera->position.x = (cTransformableCar->position.x + 40 * cos(((cTransformableCar->rotation.y - cCamera->rotExtraY) * PI) / 180.0));
    cTransformableCamera->position.z = (cTransformableCar->position.z - 40 * sin(((cTransformableCar->rotation.y - cCamera->rotExtraY) * PI) / 180.0));
}



//Entra cuando se presiona la I
void Physics::Accelerate(Car *car, Camera *cam) {
    // versión anterior
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());
    //auto cSpeed = static_cast<CSpeed *>(car->GetComponent(CompType::SpeedComp).get());
    auto cNitro = static_cast<CNitro *>(car->GetComponent(CompType::NitroComp).get());
    //Aumentamos la velocidad
    if (cNitro->activePowerUp == false) {
        cCar->speed += cCar->acceleration;
        if (cCar->speed > cCar->maxSpeed) {
            cCar->speed -= cCar->acceleration * 4.0;
            if (cCar->speed < cCar->maxSpeed)
                cCar->speed = cCar->maxSpeed;
        }
    } else {
        cCar->speed += cNitro->nitroAcceleration;
        if (cCar->speed > cNitro->nitroMaxSpeed) {
            cCar->speed = cNitro->nitroMaxSpeed;
        }
    }

    // versión nueva
    /*auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());
    auto cSpeed = static_cast<CSpeed *>(car->GetComponent(CompType::SpeedComp).get());
    auto cNitro = static_cast<CNitro *>(car->GetComponent(CompType::NitroComp).get());
    //Aumentamos la velocidad
    if (cNitro->activePowerUp == false) {
        cSpeed->speed += cCar->acceleration;
        if (cSpeed->speed > cCar->maxSpeed) {
            cSpeed->speed -= cCar->acceleration * 4.0;
            if (cSpeed->speed < cCar->maxSpeed)
                cSpeed->speed = cCar->maxSpeed;
        }
    } else {
        cSpeed->speed += cNitro->nitroAcceleration;
        if (cSpeed->speed > cNitro->nitroMaxSpeed) {
            cSpeed->speed = cNitro->nitroMaxSpeed;
        }
    }*/
}

//Entra cuando se presiona la O
void Physics::Decelerate(Car *car, Camera *cam) {
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());
    auto cNitro = static_cast<CNitro *>(car->GetComponent(CompType::NitroComp).get());
    //Reducimos la velocidad
    if (cNitro->activePowerUp == false) {
        cCar->speed -= cCar->slowDown;
        if (cCar->speed < cCar->reverseMaxSpeed) {  // no se supere la velocidad marcha atras
            cCar->speed = cCar->reverseMaxSpeed;
        }
    } else {
        cCar->speed += cNitro->nitroAcceleration;
        if (cCar->speed > cNitro->nitroMaxSpeed) {
            cCar->speed = cNitro->nitroMaxSpeed;
        }
    }
}

//Entra cuando se presiona la A
void Physics::TurnLeft(Car *car, Camera *cam) {
    //Componentes de la camara
    auto cCamera = static_cast<CCamera*>(cam->GetComponent(CompType::CameraComp).get());
    //Componentes del coche
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());

    if (cCar->speed >= cCar->maxSpeed*0.15) {
        if (cCar->wheelRotation > -cCar->maxWheelRotation) {
            //Aumentamos la rotacion hacia la izquierda
            cCar->wheelRotation -= cCar->incrementWheelRotation;
        }

        if (cCamera->rotExtraY > -(cCar->maxWheelRotation + cCamera->rotExtraCamera)) {
            cCamera->rotExtraY -= cCar->incrementWheelRotation;
        }
    } else if (cCar->speed <= -cCar->maxSpeed*0.15) {
        if (cCar->wheelRotation > -cCar->maxWheelRotation) {
            //Aumentamos la rotacion hacia la izquierda
            cCar->wheelRotation -= cCar->incrementWheelRotation;
        }
        if (cCamera->rotExtraY > -(cCar->maxWheelRotation + cCamera->rotExtraCamera)) {
            cCamera->rotExtraY -= cCar->incrementWheelRotation;
        }
    } else {  // la rueda vuelve a su sitio original al no dejarte rotar
        if (cCar->wheelRotation >= cCar->decrementWheelRotation) {
            cCar->wheelRotation -= cCar->decrementWheelRotation;
        } else if (cCar->wheelRotation <= -cCar->decrementWheelRotation) {
            cCar->wheelRotation += cCar->decrementWheelRotation;
        } else {
            cCar->wheelRotation = 0;
        }

        if (cCamera->rotExtraY >= cCar->decrementWheelRotation) {
            cCamera->rotExtraY -= cCar->decrementWheelRotation;
        } else if (cCamera->rotExtraY <= -cCar->decrementWheelRotation) {
            cCamera->rotExtraY += cCar->decrementWheelRotation;
        } else {
            cCamera->rotExtraY = 0;
        }
    }
}

//Entra cuando se presiona la D
void Physics::TurnRight(Car *car, Camera *cam) {
    //Componentes de la camara
    auto cCamera = static_cast<CCamera *>(cam->GetComponent(CompType::CameraComp).get());

    //Componentes del coche
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());

    if (cCar->speed >= cCar->maxSpeed*0.15) {
        if (cCar->wheelRotation < cCar->maxWheelRotation) {
            //Aumentamos la rotacion hacia la derecha
            cCar->wheelRotation += cCar->incrementWheelRotation;
        }

        if (cCamera->rotExtraY < (cCar->maxWheelRotation + cCamera->rotExtraCamera)) {
            cCamera->rotExtraY += cCar->incrementWheelRotation;
        }
    } else if (cCar->speed <= -cCar->maxSpeed*0.15) {
        if (cCar->wheelRotation < cCar->maxWheelRotation) {
            //Aumentamos la rotacion hacia la derecha
            cCar->wheelRotation += cCar->incrementWheelRotation;
        }
        if (cCamera->rotExtraY < (cCar->maxWheelRotation + cCamera->rotExtraCamera)) {
            cCamera->rotExtraY += cCar->incrementWheelRotation;
        }
    } else {  // la rueda vuelve a su sitio original al no dejarte rotar
        if (cCar->wheelRotation >= cCar->decrementWheelRotation) {
            cCar->wheelRotation -= cCar->decrementWheelRotation;
        } else if (cCar->wheelRotation <= -cCar->decrementWheelRotation) {
            cCar->wheelRotation += cCar->decrementWheelRotation;
        } else {
            cCar->wheelRotation = 0;
        }

        if (cCamera->rotExtraY >= cCar->decrementWheelRotation) {
            cCamera->rotExtraY -= cCar->decrementWheelRotation;
        } else if (cCamera->rotExtraY <= -cCar->decrementWheelRotation) {
            cCamera->rotExtraY += cCar->decrementWheelRotation;
        } else {
            cCamera->rotExtraY = 0;
        }
    }
}

//Aqui entra cuando no se esta presionando ni I ni O
void Physics::NotAcceleratingOrDecelerating(Car *car, Camera *cam) {
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());
    auto cNitro = static_cast<CNitro *>(car->GetComponent(CompType::NitroComp).get());

    if (cNitro->activePowerUp == false) {
        if (cCar->speed > 0) {
            cCar->speed -= cCar->friction;
            if (cCar->speed < 0)
                cCar->speed = 0;
        } else if (cCar->speed < 0) {
            cCar->speed += cCar->friction;
            if (cCar->speed > 0)
                cCar->speed = 0;
        }
    } else {
        cCar->speed += cNitro->nitroAcceleration;
        if (cCar->speed > cNitro->nitroMaxSpeed) {
            cCar->speed = cNitro->nitroMaxSpeed;
        }
    }
}


//Aqui entra cuando no se esta presionando ni A ni D
void Physics::NotTurning(Car *car, Camera *cam) {
    //Componentes de la camara
    auto cCamera = static_cast<CCamera *>(cam->GetComponent(CompType::CameraComp).get());

    //Componentes del coche
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());

    if (cCar->wheelRotation >= cCar->decrementWheelRotation) {
        cCar->wheelRotation -= cCar->decrementWheelRotation;
    } else if (cCar->wheelRotation <= -cCar->decrementWheelRotation) {
        cCar->wheelRotation += cCar->decrementWheelRotation;
    } else {
        cCar->wheelRotation = 0;
    }

    if (cCamera->rotExtraY >= cCar->decrementWheelRotation) {
        cCamera->rotExtraY -= cCar->decrementWheelRotation;
    } else if (cCamera->rotExtraY <= -cCar->decrementWheelRotation) {
        cCamera->rotExtraY += cCar->decrementWheelRotation;
    } else {
        cCamera->rotExtraY = 0;
    }
}