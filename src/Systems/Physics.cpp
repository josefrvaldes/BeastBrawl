#include "Physics.h"
#include "../Entities/Camera.h"
#include "../Entities/Car.h"

Physics::Physics(float *_deltaTime) : deltaTime(_deltaTime) {
}

//Calcula la posicion del coche (duda con las formulas preguntar a Jose)
void CalculatePosition(CCar *cCar, CTransformable *cTransformable, float *deltaTime) {
    /*float angleRotation = (cTransformable->rotation.y * PI) / 180.0;

    //Modificamos la posicion en X y Z en funcion del angulo
    cTransformable->position.x -= cos(angleRotation) * cCar->speed * *deltaTime;
    cTransformable->position.z += sin(angleRotation) * cCar->speed * *deltaTime;

    //Si tiene rotacion, rotamos el coche
    if (cCar->wheelRotation != 0) {
        cTransformable->rotation.y += cCar->wheelRotation * 0.20;
        if (cTransformable->rotation.y >= 360.0)
            cTransformable->rotation.y -= 360.0;
        else if (cTransformable->rotation.y < 0.0)
            cTransformable->rotation.y += 360.0;
    }/
    //std::cout << "Angulo rot: " << cTransformable->rotation.y << std::endl;
    */
    float angleRotation = (cTransformable->rotation.y * PI) / 180.0;
    float delta = *deltaTime;
    cout << "El deltaTime es " << delta << endl;

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
void CalculatePositionReverse(CCar *cCar, CTransformable *cTransformable, float *deltaTime) {
    float angleRotation = (cTransformable->rotation.y * PI) / 180.0;

    float delta = *deltaTime;
    cout << "El deltaTime es (reverse)" << delta << endl;

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
void CalculatePositionCamera(CTransformable *cTransformableCar, CTransformable *cTransformableCamera, CCamera *cCamera) {
    //cout << "Estamos en physics CalculatePositionCamera" << endl;
    cTransformableCamera->position.y = cTransformableCar->position.y + 20;
    cTransformableCamera->position.x = (cTransformableCar->position.x + 40 * cos(((cTransformableCar->rotation.y - cCamera->rotExtraY) * PI) / 180.0));
    cTransformableCamera->position.z = (cTransformableCar->position.z - 40 * sin(((cTransformableCar->rotation.y - cCamera->rotExtraY) * PI) / 180.0));
}

//Calcula la posicion de la camara (duda con las formulas preguntar a Jose)
/*void CalculatePositionCamera(CTransformable *cTransformableCar, CTransformable *cTransformableCamera, CCamera *cCamera) {
    cTransformableCamera->position.y = cTransformableCar->position.y + 20;
    cTransformableCamera->position.x = (cTransformableCar->position.x - 40 * sin(((cTransformableCar->rotation.y - cCamera->rotExtraY) * PI) / 180.0));
    cTransformableCamera->position.z = (cTransformableCar->position.z - 40 * cos(((cTransformableCar->rotation.y - cCamera->rotExtraY) * PI) / 180.0));
}*/

//Entra cuando se presiona la I
void Physics::Accelerate(Car *car, Camera *cam) {
    //cout << "Estamos en physics Accelerate" << endl;
    //Componentes de la camara
    auto componentsCam = cam->GetComponents();
    auto cCamera = static_cast<CCamera *>(componentsCam[CompType::CameraComp].get());
    auto cTransformableCam = static_cast<CTransformable *>(componentsCam[CompType::TransformableComp].get());

    //Componentes del coche
    auto components = car->GetComponents();
    auto cTransformable = static_cast<CTransformable *>(components[CompType::TransformableComp].get());
    auto cCar = static_cast<CCar *>(components[CompType::CarComp].get());

    //Aumentamos la velocidad
    cCar->speed += cCar->acceleration;
    if (cCar->speed > cCar->maxSpeed) {
        cCar->speed = cCar->maxSpeed;
    }

    if (cCar->speed >= 0)
        CalculatePosition(cCar, cTransformable, deltaTime);
    else
        CalculatePositionReverse(cCar, cTransformable, deltaTime);
    CalculatePositionCamera(cTransformable, cTransformableCam, cCamera);
}

//Entra cuando se presiona la O
void Physics::Decelerate(Car *car, Camera *cam) {
    //Componentes de la camara
    auto componentsCam = cam->GetComponents();
    auto cCamera = static_cast<CCamera *>(componentsCam[CompType::CameraComp].get());
    auto cTransformableCam = static_cast<CTransformable *>(componentsCam[CompType::TransformableComp].get());

    //Componentes del coche
    auto components = car->GetComponents();
    auto cTransformable = static_cast<CTransformable *>(components[CompType::TransformableComp].get());
    auto cCar = static_cast<CCar *>(components[CompType::CarComp].get());

    //Reducimos la velocidad
    cCar->speed -= cCar->slowDown;
    if (cCar->speed < cCar->reverseMaxSpeed) {  // no se supere la velocidad marcha atras
        cCar->speed = cCar->reverseMaxSpeed;
    }

    if (cCar->speed >= 0)
        CalculatePosition(cCar, cTransformable, deltaTime);
    else
        CalculatePositionReverse(cCar, cTransformable, deltaTime);
    CalculatePositionCamera(cTransformable, cTransformableCam, cCamera);
}

//Entra cuando se presiona la A
void Physics::TurnLeft(Car *car, Camera *cam) {
    //Componentes de la camara
    auto componentsCam = cam->GetComponents();
    auto cCamera = static_cast<CCamera *>(componentsCam[CompType::CameraComp].get());
    auto cTransformableCam = static_cast<CTransformable *>(componentsCam[CompType::TransformableComp].get());

    //Componentes del coche
    auto components = car->GetComponents();
    auto cTransformable = static_cast<CTransformable *>(components[CompType::TransformableComp].get());
    auto cCar = static_cast<CCar *>(components[CompType::CarComp].get());

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

    if (cCar->speed >= 0)
        CalculatePosition(cCar, cTransformable, deltaTime);
    else
        CalculatePositionReverse(cCar, cTransformable, deltaTime);
    CalculatePositionCamera(cTransformable, cTransformableCam, cCamera);
}

//Entra cuando se presiona la D
void Physics::TurnRight(Car *car, Camera *cam) {
    //Componentes de la camara
    auto componentsCam = cam->GetComponents();
    auto cCamera = static_cast<CCamera *>(componentsCam[CompType::CameraComp].get());
    auto cTransformableCam = static_cast<CTransformable *>(componentsCam[CompType::TransformableComp].get());

    //Componentes del coche
    auto components = car->GetComponents();
    auto cTransformable = static_cast<CTransformable *>(components[CompType::TransformableComp].get());
    auto cCar = static_cast<CCar *>(components[CompType::CarComp].get());

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

    if (cCar->speed >= 0)
        CalculatePosition(cCar, cTransformable, deltaTime);
    else
        CalculatePositionReverse(cCar, cTransformable, deltaTime);
    CalculatePositionCamera(cTransformable, cTransformableCam, cCamera);
}

//Aqui entra cuando no se esta presionando ni I ni O
void Physics::NotAcceleratingOrDecelerating(Car *car, Camera *cam) {
    //Componentes de la camara
    auto componentsCam = cam->GetComponents();
    auto cCamera = static_cast<CCamera *>(componentsCam[CompType::CameraComp].get());
    auto cTransformableCam = static_cast<CTransformable *>(componentsCam[CompType::TransformableComp].get());

    //Componentes del coche
    auto components = car->GetComponents();
    auto cTransformable = static_cast<CTransformable *>(components[CompType::TransformableComp].get());
    auto cCar = static_cast<CCar *>(components[CompType::CarComp].get());

    if (cCar->speed > 0) {
        cCar->speed -= cCar->friction;
        if (cCar->speed < 0)
            cCar->speed = 0;
    } else if (cCar->speed < 0) {
        cCar->speed += cCar->friction;
        if (cCar->speed > 0)
            cCar->speed = 0;
    }

    if (cCar->speed >= 0)
        CalculatePosition(cCar, cTransformable, deltaTime);
    else
        CalculatePositionReverse(cCar, cTransformable, deltaTime);
    CalculatePositionCamera(cTransformable, cTransformableCam, cCamera);
}

//Aqui entra cuando no se esta presionando ni A ni D
void Physics::NotTurning(Car *car, Camera *cam) {
    //Componentes de la camara
    auto componentsCam = cam->GetComponents();
    auto cCamera = static_cast<CCamera *>(componentsCam[CompType::CameraComp].get());
    auto cTransformableCam = static_cast<CTransformable *>(componentsCam[CompType::TransformableComp].get());

    //Componentes del coche
    auto components = car->GetComponents();
    auto cTransformable = static_cast<CTransformable *>(components[CompType::TransformableComp].get());
    auto cCar = static_cast<CCar *>(components[CompType::CarComp].get());

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

    if (cCar->speed >= 0)
        CalculatePosition(cCar, cTransformable, deltaTime);
    else
        CalculatePositionReverse(cCar, cTransformable, deltaTime);
    CalculatePositionCamera(cTransformable, cTransformableCam, cCamera);
}