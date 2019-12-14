#include "Physics.h"
#include "../Entities/Camera.h"
#include "../Entities/Car.h"

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
    //Aumentamos la velocidad
    cCar->speed += cCar->acceleration;
    if (cCar->speed > cCar->maxSpeed) {
        cCar->speed = cCar->maxSpeed;
    }
}


//Entra cuando se presiona la O
void Physics::Decelerate(Car *car, Camera *cam) {
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());
    //Reducimos la velocidad
    cCar->speed -= cCar->slowDown;
    if (cCar->speed < cCar->reverseMaxSpeed) {  // no se supere la velocidad marcha atras
        cCar->speed = cCar->reverseMaxSpeed;
    }
}



//Entra cuando se presiona la A
void Physics::TurnLeft(Car *car, Camera *cam) {
    //Componentes de la camara
    auto cCamera = static_cast<CCamera*>(cam->GetComponent(CompType::CameraComp).get());

    //Componentes del coche
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());

    if (cCar->speed >= 3) {
        if (cCar->wheelRotation > -cCar->maxWheelRotation) {
            //Aumentamos la rotacion hacia la izquierda
            cCar->wheelRotation -= cCar->incrementWheelRotation;
        }

        if (cCamera->rotExtraY > -(cCar->maxWheelRotation+cCamera->rotExtraCamera)) {
            cCamera->rotExtraY -= cCar->incrementWheelRotation;
        }
    } else if (cCar->speed <= -3) {
        if (cCar->wheelRotation > -cCar->maxWheelRotation) {
            //Aumentamos la rotacion hacia la izquierda
            cCar->wheelRotation -= cCar->incrementWheelRotation;
        }
        if (cCamera->rotExtraY > -(cCar->maxWheelRotation+cCamera->rotExtraCamera)) {
            cCamera->rotExtraY -= cCar->incrementWheelRotation;
        }
    } else {    // la rueda vuelve a su sitio original al no dejarte rotar
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
    auto cCamera = static_cast<CCamera*>(cam->GetComponent(CompType::CameraComp).get());

    //Componentes del coche
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());

    if (cCar->speed >= 3) {
        if (cCar->wheelRotation < cCar->maxWheelRotation) {
            //Aumentamos la rotacion hacia la derecha
            cCar->wheelRotation += cCar->incrementWheelRotation;
        }

        if (cCamera->rotExtraY < (cCar->maxWheelRotation+cCamera->rotExtraCamera)) {
            cCamera->rotExtraY += cCar->incrementWheelRotation;
        }
    } else if (cCar->speed <= -3) {
        if (cCar->wheelRotation < cCar->maxWheelRotation) {
            //Aumentamos la rotacion hacia la derecha
            cCar->wheelRotation += cCar->incrementWheelRotation;
        }
        if (cCamera->rotExtraY < (cCar->maxWheelRotation+cCamera->rotExtraCamera)) {
            cCamera->rotExtraY += cCar->incrementWheelRotation;
        }
    } else {         // la rueda vuelve a su sitio original al no dejarte rotar
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

    if (cCar->speed > 0) {
        cCar->speed -= cCar->friction;
        if (cCar->speed < 0)
            cCar->speed = 0;
    } else if (cCar->speed < 0) {
        cCar->speed += cCar->friction;
        if (cCar->speed > 0)
            cCar->speed = 0;
    }
}

//Aqui entra cuando no se esta presionando ni A ni D
void Physics::NotTurning(Car *car, Camera *cam) {
    //Componentes de la camara
    auto cCamera = static_cast<CCamera*>(cam->GetComponent(CompType::CameraComp).get());

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