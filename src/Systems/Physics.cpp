#include "Physics.h"
#include "../Entities/Camera.h"
#include "../Entities/Car.h"
#include <chrono>

using namespace chrono;

Physics::Physics(float *_deltaTime) : deltaTime(_deltaTime) {
}


void Physics::update(Car* car, Camera* cam){
    // actualizar posiciones
    auto cTransformable = static_cast<CTransformable *>(car->GetComponent(CompType::TransformableComp).get());
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());
    auto cCamera = static_cast<CCamera*>(cam->GetComponent(CompType::CameraComp).get());
    auto cTransformableCam = static_cast<CTransformable *>(cam->GetComponent(CompType::TransformableComp).get());
    if (cCar->speed >= 0)
        CalculatePosition(cCar, cTransformable, deltaTime);
    else
        CalculatePositionReverse(cCar, cTransformable, deltaTime);
    CalculatePositionCamera(cTransformable, cTransformableCam, cCamera);



    // To-Do: actualizar componentes PowerUps (nitro, robojorobo y escudo)...
    auto cNitro = static_cast<CNitro *>(car->GetComponent(CompType::NitroComp).get());
    if(cNitro->activePowerUp==true && duration_cast<milliseconds>(system_clock::now() - cNitro->timeStart).count() > cNitro->durationTime){  // comprueba el tiempo desde que se lanzo
        cNitro->deactivePowerUp();
    }

    auto cShield = static_cast<CShield *>(car->GetComponent(CompType::ShieldComp).get());
    if(cShield->activePowerUp==true && duration_cast<milliseconds>(system_clock::now() - cShield->timeStart).count() > cShield->durationTime){  // comprueba el tiempo desde que se lanzo
        cShield->deactivePowerUp();
    }
}




//Calcula la posicion del coche (duda con las formulas preguntar a Jose)
void Physics::CalculatePosition(CCar *cCar, CTransformable *cTransformable, float *deltaTime) {
    float angleRotation = (cTransformable->rotation.y * PI) / 180.0;
    float delta = *deltaTime;
    // cout << "El deltaTime es " << delta << endl;

    //Modificamos la posicion en X y Z en funcion del angulo
    cTransformable->position.x -= cos(angleRotation) * cCar->speed * delta;
    cTransformable->position.z += sin(angleRotation) * cCar->speed * delta;

    //Si tiene rotacion, rotamos el coche
    if (cCar->wheelRotation != 0) {
        cTransformable->rotation.y += cCar->wheelRotation * 0.20;
        if (cTransformable->rotation.y >= 360.0)
            cTransformable->rotation.y -= 360.0;
        else if (cTransformable->rotation.y < 0.0)
            cTransformable->rotation.y += 360.0;
    }
}

//Calcula la posicion del coche (duda con las formulas preguntar a Jose)
void Physics::CalculatePositionReverse(CCar *cCar, CTransformable *cTransformable, float *deltaTime) {
    float angleRotation = (cTransformable->rotation.y * PI) / 180.0;

    float delta = *deltaTime;
    // cout << "El deltaTime es (reverse)" << delta << endl;

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
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());
    auto cNitro = static_cast<CNitro *>(car->GetComponent(CompType::NitroComp).get());
    //Aumentamos la velocidad
    if(cNitro->activePowerUp==false){
        cCar->speed += cCar->acceleration;
        if (cCar->speed > cCar->maxSpeed) {
            cCar->speed -= cCar->acceleration*4.0;
            if(cCar->speed < cCar->maxSpeed)
                cCar->speed = cCar->maxSpeed;
        }
    }else{
        cCar->speed += cNitro->nitroAcceleration;
        if (cCar->speed > cNitro->nitroMaxSpeed) {
            cCar->speed = cNitro->nitroMaxSpeed;
        }  
    }
}


//Entra cuando se presiona la O
void Physics::Decelerate(Car *car, Camera *cam) {
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());
    auto cNitro = static_cast<CNitro *>(car->GetComponent(CompType::NitroComp).get());
    //Reducimos la velocidad
    if(cNitro->activePowerUp==false){
        cCar->speed -= cCar->slowDown;
        if (cCar->speed < cCar->reverseMaxSpeed) {  // no se supere la velocidad marcha atras
            cCar->speed = cCar->reverseMaxSpeed;
        }
    }else{
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

    if (cCar->speed >= 3) {
        if (cCar->wheelRotation > -10) {
            //Aumentamos la rotacion hacia la izquierda
            cCar->wheelRotation -= 0.5;
        }

        if (cCamera->rotExtraY > -15) {
            cCamera->rotExtraY -= 0.5;
        }
    } else if (cCar->speed <= -3) {
        if (cCar->wheelRotation > -10) {
            //Aumentamos la rotacion hacia la izquierda
            cCar->wheelRotation -= 0.5;
        }
        if (cCamera->rotExtraY > -15) {
            cCamera->rotExtraY -= 0.5;
        }
    } else {
        if (cCar->wheelRotation >= 0.7) {
            cCar->wheelRotation -= 0.7;
        } else if (cCar->wheelRotation <= -0.7) {
            cCar->wheelRotation += 0.7;
        } else {
            cCar->wheelRotation = 0;
        }

        if (cCamera->rotExtraY >= 0.7) {
            cCamera->rotExtraY -= 0.7;
        } else if (cCamera->rotExtraY <= -0.7) {
            cCamera->rotExtraY += 0.7;
        } else {
            cCamera->rotExtraY = 0;
        }
    }
}



//Entra cuando se presiona la D
void Physics::TurnRight(Car *car, Camera *cam) {
    //Componentes de la camara
    auto cCamera = static_cast<CCamera*>(cam->GetComponent(CompType::CameraComp).get());

    //Componentes del coche
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());

    if (cCar->speed >= 3) {
        if (cCar->wheelRotation < 10) {
            //Aumentamos la rotacion hacia la derecha
            cCar->wheelRotation += 0.5;
        }

        if (cCamera->rotExtraY < 15) {
            cCamera->rotExtraY += 0.5;
        }
    } else if (cCar->speed <= -3) {
        if (cCar->wheelRotation < 10) {
            //Aumentamos la rotacion hacia la derecha
            cCar->wheelRotation += 0.5;
        }
        if (cCamera->rotExtraY < 15) {
            cCamera->rotExtraY += 0.5;
        }
    } else {
        if (cCar->wheelRotation >= 0.7) {
            cCar->wheelRotation -= 0.7;
        } else if (cCar->wheelRotation <= -0.7) {
            cCar->wheelRotation += 0.7;
        } else {
            cCar->wheelRotation = 0;
        }

        if (cCamera->rotExtraY >= 0.7) {
            cCamera->rotExtraY -= 0.7;
        } else if (cCamera->rotExtraY <= -0.7) {
            cCamera->rotExtraY += 0.7;
        } else {
            cCamera->rotExtraY = 0;
        }
    }
}



//Aqui entra cuando no se esta presionando ni I ni O
void Physics::NotAcceleratingOrDecelerating(Car *car, Camera *cam) {
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());
    auto cNitro = static_cast<CNitro *>(car->GetComponent(CompType::NitroComp).get());

    if(cNitro->activePowerUp==false){
        if (cCar->speed > 0) {
            cCar->speed -= cCar->friction;
            if (cCar->speed < 0)
                cCar->speed = 0;
        } else if (cCar->speed < 0) {
            cCar->speed += cCar->friction;
            if (cCar->speed > 0)
                cCar->speed = 0;
        }
    }else{
        cCar->speed += cNitro->nitroAcceleration;
        if (cCar->speed > cNitro->nitroMaxSpeed) {
            cCar->speed = cNitro->nitroMaxSpeed;
        }  
    }
}

//Aqui entra cuando no se esta presionando ni A ni D
void Physics::NotTurning(Car *car, Camera *cam) {
    //Componentes de la camara
    auto cCamera = static_cast<CCamera*>(cam->GetComponent(CompType::CameraComp).get());

    //Componentes del coche
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());

    if (cCar->wheelRotation >= 0.7) {
        cCar->wheelRotation -= 0.7;
    } else if (cCar->wheelRotation <= -0.7) {
        cCar->wheelRotation += 0.7;
    } else {
        cCar->wheelRotation = 0;
    }

    if (cCamera->rotExtraY >= 0.7) {
        cCamera->rotExtraY -= 0.7;
    } else if (cCamera->rotExtraY <= -0.7) {
        cCamera->rotExtraY += 0.7;
    } else {
        cCamera->rotExtraY = 0;
    }
}