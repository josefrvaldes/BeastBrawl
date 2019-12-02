#include "Physics.h"

void CalculatePosition(CCar* cCar, CTransformable* cTransformable, Data d);
void CalculatePositionReverse(CCar* cCar, CTransformable* cTransformable, Data d);
void CalculatePositionCamera(CTransformable* cTransformableCar,CTransformable* cTransformableCamera, CCamera* cCamera);

//TODO: Cambiar en los punteros a funciones en vez de ir pasandole datos por Event cogerlos del EntityManager
// PUNTEROS A FUNCIONES
void Accelerate(Data d);
void Decelerate(Data d);
void TurnLeft(Data d);
void TurnRight(Data d);
void NotAcceleratingOrDecelerating(Data d);
void NotTurning(Data d);

Physics::Physics(){
    eventManager = EventManager::GetInstance();

    //Se suscriben los listeners
    eventManager->Suscribe(Listener {EventType::PRESS_I,Accelerate,"accelerate"});
    eventManager->Suscribe(Listener {EventType::PRESS_A,TurnLeft,"turnLeft"});
    eventManager->Suscribe(Listener {EventType::PRESS_D,TurnRight,"turnRight"});
    eventManager->Suscribe(Listener {EventType::PRESS_O,Decelerate,"decelerate"});
    eventManager->Suscribe(Listener {EventType::NO_I_O_PRESS,NotAcceleratingOrDecelerating,"notAcceleratingOrDecelerating"});
    eventManager->Suscribe(Listener {EventType::NO_A_D_PRESS,NotTurning,"notTurning"});
}


//PUNTEROS A FUNCIONES

//Entra cuando se presiona la I
void Accelerate(Data d){
    //Componentes de la camara
    auto componentsCam = d.camera->GetComponents();
    auto mapCamera = componentsCam.find(CompType::CameraComp);
    auto cCamera = static_cast<CCamera*>(mapCamera->second);
    auto mapTransformCamera = componentsCam.find(CompType::TransformableComp);
	auto cTransformableCam = static_cast<CTransformable*>(mapTransformCamera->second);
    //Guardamos en variables los componentes
    auto components = d.gameObject->GetComponents();
    auto mapTransform = components.find(CompType::TransformableComp);
	auto cTransformable = static_cast<CTransformable*>(mapTransform->second);
    
    auto mapCar = components.find(CompType::CarComp);
    auto cCar        = static_cast<CCar*>(mapCar->second);

    //Aumentamos la velocidad
    cCar->speed += cCar->acceleration;
    if(cCar->speed > cCar->maxSpeed){
        cCar->speed = cCar->maxSpeed;
    }

    if(cCar->speed>=0)
        CalculatePosition(cCar,cTransformable, d);
    else
        CalculatePositionReverse(cCar,cTransformable, d);
    CalculatePositionCamera(cTransformable,cTransformableCam,cCamera);
}

//Entra cuando se presiona la O
void Decelerate(Data d){
    //Componentes de la camara
    auto componentsCam = d.camera->GetComponents();
    auto mapCamera = componentsCam.find(CompType::CameraComp);
    auto cCamera = static_cast<CCamera*>(mapCamera->second);
    auto mapTransformCamera = componentsCam.find(CompType::TransformableComp);
	auto cTransformableCam = static_cast<CTransformable*>(mapTransformCamera->second);
    //Guardamos en variables los componentes
    auto components = d.gameObject->GetComponents();
    auto mapTransform = components.find(CompType::TransformableComp);
	auto cTransformable = static_cast<CTransformable*>(mapTransform->second);
    auto mapCar = components.find(CompType::CarComp);
    auto cCar        = static_cast<CCar*>(mapCar->second);

    //Reducimos la velocidad
    cCar->speed -= cCar->acceleration;
    if(cCar->speed < cCar->reverseMaxSpeed){ // no se supere la velocidad marcha atras
        cCar->speed = cCar->reverseMaxSpeed;
    }
    
    if(cCar->speed>=0)
        CalculatePosition(cCar,cTransformable, d);
    else
        CalculatePositionReverse(cCar,cTransformable, d);
    CalculatePositionCamera(cTransformable,cTransformableCam,cCamera);
}

//Entra cuando se presiona la A
void TurnLeft(Data d){

    //Componentes de la camara
    auto componentsCam = d.camera->GetComponents();
    auto mapCamera = componentsCam.find(CompType::CameraComp);
    auto cCamera = static_cast<CCamera*>(mapCamera->second);
    auto mapTransformCamera = componentsCam.find(CompType::TransformableComp);
	auto cTransformableCam = static_cast<CTransformable*>(mapTransformCamera->second);


    //Componentes del coche
    auto components = d.gameObject->GetComponents();
    auto mapTransform = components.find(CompType::TransformableComp);
	auto cTransformable = static_cast<CTransformable*>(mapTransform->second);   
    auto mapCar = components.find(CompType::CarComp);
    auto cCar        = static_cast<CCar*>(mapCar->second);


    if(cCar->speed >= 3){
        if(cCar->wheelRotation > -10){
            //Aumentamos la rotacion hacia la izquierda
            cCar->wheelRotation -= 0.5;
        }

        if(cCamera->rotExtraY > -15) {
            cCamera->rotExtraY -= 0.5;
        }
    } else if(cCar->speed <= -3){
        if(cCar->wheelRotation > -10){
            //Aumentamos la rotacion hacia la izquierda
            cCar->wheelRotation -= 0.5;
        }
    }
    
    if(cCar->speed>=0)
        CalculatePosition(cCar,cTransformable, d);
    else
        CalculatePositionReverse(cCar,cTransformable, d);
    CalculatePositionCamera(cTransformable,cTransformableCam,cCamera);

}

//Entra cuando se presiona la D
void TurnRight(Data d){
    //Componentes de la camara
    auto componentsCam = d.camera->GetComponents();
    auto mapCamera = componentsCam.find(CompType::CameraComp);
    auto cCamera = static_cast<CCamera*>(mapCamera->second);
    auto mapTransformCamera = componentsCam.find(CompType::TransformableComp);
	auto cTransformableCam = static_cast<CTransformable*>(mapTransformCamera->second);

    //Guardamos en variables los componentes
    auto components = d.gameObject->GetComponents();
    auto mapTransform = components.find(CompType::TransformableComp);
	auto cTransformable = static_cast<CTransformable*>(mapTransform->second);
    
    auto mapCar = components.find(CompType::CarComp);
    auto cCar        = static_cast<CCar*>(mapCar->second);


    if(cCar->speed >= 3){
        if(cCar->wheelRotation < 10){
            //Aumentamos la rotacion hacia la derecha
            cCar->wheelRotation += 0.5;
        }

        if(cCamera->rotExtraY < 15){
            cCamera->rotExtraY += 0.5;
        }
    }else if(cCar->speed <= -3){
        if(cCar->wheelRotation < 10){
            //Aumentamos la rotacion hacia la derecha
            cCar->wheelRotation += 0.5;
        }
    }
    
    if(cCar->speed>=0)
        CalculatePosition(cCar,cTransformable, d);
    else
        CalculatePositionReverse(cCar,cTransformable, d);
    CalculatePositionCamera(cTransformable,cTransformableCam,cCamera);
}

//Aqui entra cuando no se esta presionando ni I ni O
void NotAcceleratingOrDecelerating(Data d){
    //Componentes de la camara
    auto componentsCam = d.camera->GetComponents();
    auto mapCamera = componentsCam.find(CompType::CameraComp);
    auto cCamera = static_cast<CCamera*>(mapCamera->second);
    auto mapTransformCamera = componentsCam.find(CompType::TransformableComp);
	auto cTransformableCam = static_cast<CTransformable*>(mapTransformCamera->second);
    //Guardamos en variables los componentes
    auto components = d.gameObject->GetComponents();
    auto mapTransform = components.find(CompType::TransformableComp);
	auto cTransformable = static_cast<CTransformable*>(mapTransform->second);
    
    auto mapCar = components.find(CompType::CarComp);
    auto cCar        = static_cast<CCar*>(mapCar->second);

    if(cCar->speed > 0){
        cCar->speed -= cCar->friction;
        if(cCar->speed < 0)
            cCar->speed = 0;
    }else if(cCar->speed < 0){
        cCar->speed += cCar->friction;
        if(cCar->speed > 0)
            cCar->speed = 0;
    }

    if(cCar->speed>=0)
        CalculatePosition(cCar,cTransformable, d);
    else
        CalculatePositionReverse(cCar,cTransformable, d);
    CalculatePositionCamera(cTransformable,cTransformableCam,cCamera);
}

//Aqui entra cuando no se esta presionando ni A ni D
void NotTurning(Data d){
    //Componentes de la camara
    auto componentsCam = d.camera->GetComponents();
    auto mapCamera = componentsCam.find(CompType::CameraComp);
    auto cCamera = static_cast<CCamera*>(mapCamera->second);
    auto mapTransformCamera = componentsCam.find(CompType::TransformableComp);
	auto cTransformableCam = static_cast<CTransformable*>(mapTransformCamera->second);

    //Componentes del coche
    auto components = d.gameObject->GetComponents();
    auto mapTransform = components.find(CompType::TransformableComp);
	auto cTransformable = static_cast<CTransformable*>(mapTransform->second);
    auto mapCar = components.find(CompType::CarComp);
    auto cCar        = static_cast<CCar*>(mapCar->second);

    if(cCar->wheelRotation >= 0.7){
        cCar->wheelRotation -= 0.7;
    }else if(cCar->wheelRotation <= -0.7){
        cCar->wheelRotation += 0.7;
    }else{
        cCar->wheelRotation = 0;
    }

    if(cCamera->rotExtraY >= 0.7){
        cCamera->rotExtraY -= 0.7;
    }else if(cCamera->rotExtraY <= -0.7){
        cCamera->rotExtraY += 0.7;
    }else{
        cCamera->rotExtraY = 0;        
    }

    if(cCar->speed>=0)
        CalculatePosition(cCar,cTransformable, d);
    else
        CalculatePositionReverse(cCar,cTransformable, d);
    CalculatePositionCamera(cTransformable,cTransformableCam,cCamera);
}

//Calcula la posicion del coche (duda con las formulas preguntar a Jose)
void CalculatePosition(CCar* cCar, CTransformable* cTransformable, Data d){
    float angleRotation = (cTransformable->rotY * PI) /180.0;
    
    //Modificamos la posicion en X y Z en funcion del angulo
    cTransformable->posX += sin(angleRotation) * cCar->speed * d.deltaTime;
    cTransformable->posZ += cos(angleRotation) * cCar->speed * d.deltaTime;

    //Si tiene rotacion, rotamos el coche
    if(cCar->wheelRotation != 0){
        cTransformable->rotY += cCar->wheelRotation * 0.20;
    }
}


//Calcula la posicion del coche (duda con las formulas preguntar a Jose)
void CalculatePositionReverse(CCar* cCar, CTransformable* cTransformable, Data d){
    float angleRotation = (cTransformable->rotY * PI) /180.0;
    
    //Modificamos la posicion en X y Z en funcion del angulo
    cTransformable->posX += sin(angleRotation) * cCar->speed * d.deltaTime;
    cTransformable->posZ += cos(angleRotation) * cCar->speed * d.deltaTime;

    //Si tiene rotacion, rotamos el coche
    if(cCar->wheelRotation != 0){
        cTransformable->rotY -= cCar->wheelRotation * 0.20;
    }
}

//Calcula la posicion de la camara (duda con las formulas preguntar a Jose)
void CalculatePositionCamera(CTransformable* cTransformableCar,CTransformable* cTransformableCamera, CCamera* cCamera){
    cTransformableCamera->posY = cTransformableCar->posY + 20;
    cTransformableCamera->posX = (cTransformableCar->posX - 40 * sin(((cTransformableCar->rotY - cCamera->rotExtraY)*PI)/180.0));
    cTransformableCamera->posZ = (cTransformableCar->posZ - 40 * cos(((cTransformableCar->rotY - cCamera->rotExtraY)*PI)/180.0));
}