#include "PhysicsIA.h"


void CalculatePositionIA(CCar* cCar, CTransformable* cTransformable, Data d);
void CalculatePositionReverseIA(CCar* cCar, CTransformable* cTransformable, Data d);
void CalculatePositionCameraIA(CTransformable* cTransformableCar,CTransformable* cTransformableCamera, CCamera* cCamera);

//TODO: Cambiar en los punteros a funciones en vez de ir pasandole datos por Event cogerlos del EntityManager
// PUNTEROS A FUNCIONES
void AccelerateIA(Data d);
void DecelerateIA(Data d);
void TurnLeftIA(Data d);
void TurnRightIA(Data d);
void NotAcceleratingOrDeceleratingIA(Data d);
void NotTurningIA(Data d);

PhysicsIA::PhysicsIA(){

}

//#include <cmath>
//
//float dot(Vec3 a, Vec3 b)  //calculates dot product of a and b
//{
//    return a.x * b.x + a.y * b.y + a.z * b.z;
//}
//
//float mag(Vec3 a)  //calculates magnitude of a
//{
//    return std::sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
//}
//
//int main()
//{
//    Vec3 v1, v2;
//
//    v1.x = 203;
//    v1.y = 355;
//    v1.z = 922;
//
//    v2.x = 6;
//    v2.y = 13;
//    v2.z = 198;
//
//    float angle = std::acos(dot(v1,v2)/(mag(v1)*mag(v2)));
//}
//

//     vectorX=jugador1->getBody()->getPosition().x+32-sprite->getPosition().x;
//     vectorY=jugador1->getBody()->getPosition().y+32-sprite->getPosition().y;
//    //vMagnitud= (vectorX*vectorX)+(vectorY*vectorY);
//    //divisorUnitario=sqrt(vMagnitud);
//    //angleRads = atan2(-vectorY, vectorX);
//    //angleDegs = angleRads * 180 / PI;
//    float angle = std::acos(dot(v1,v2)/(mag(v1)*mag(v2)));

float dot(glm::vec3 a, glm::vec3 b){  //calculates dot product of a and b{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
float mag(glm::vec3 a){  //calculates magnitude of a{
    return std::sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}
glm::vec3 vector2P(glm::vec3 posAnterior, glm::vec3 posSiguiente){
    return glm::vec3(posSiguiente.x - posAnterior.x, posSiguiente.y - posAnterior.y, posSiguiente.z - posAnterior.z);
}

void calculateAngle(WayPoint* wayPointNext, Car* car){
    auto components = car->GetComponents();
    auto cTransformable = static_cast<CTransformable*>(components[CompType::TransformableComp]);
    auto cLastPosition = static_cast<CLastPosition*>(components[CompType::LastPositionComp]);
    glm::vec3 posActualCar   = cTransformable->position;
    glm::vec3 posPreviousCar = cLastPosition->position;
    glm::vec3 vecDirCar = vector2P(posPreviousCar, posActualCar);

    glm::vec3 vecWayPoint = glm::vec3(wayPointNext->getPosX(),wayPointNext->getPosY(),wayPointNext->getPosZ());
    glm::vec3 vecDirWay = vector2P(posActualCar, vecWayPoint);


    //glm::vec3 vector1 = glm::vec3(1.0f,1.0f, 1.f);
    //glm::vec3 vector2 = glm::vec3(0.0f,0.0f, 0.f);
    //float angle = std::acos(dot(vector1,vector2)/(mag(vector1)*mag(vector2)));
    float angle = std::acos(dot(vecDirCar,vecDirWay)/(mag(vecDirCar)*mag(vecDirWay)));

    std::cout << "EL ANGULO ES:     " << angle << std::endl;

}







void PhysicsIA::update(vector<WayPoint *> wayPoints, Car* car){
    //void calcularDirIA();
    std::cout << "HOLAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" << std::endl;
}

//PUNTEROS A FUNCIONES

//Entra cuando se presiona la I
void AccelerateIA(Data d){
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
        CalculatePositionIA(cCar,cTransformable, d);
    else
        CalculatePositionReverseIA(cCar,cTransformable, d);
    CalculatePositionCameraIA(cTransformable,cTransformableCam,cCamera);
}

//Entra cuando se presiona la O
void DecelerateIA(Data d){
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
        CalculatePositionIA(cCar,cTransformable, d);
    else
        CalculatePositionReverseIA(cCar,cTransformable, d);
    CalculatePositionCameraIA(cTransformable,cTransformableCam,cCamera);
}

//Entra cuando se presiona la A
void TurnLeftIA(Data d){

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
        if(cCamera->rotExtraY > -15) {
            cCamera->rotExtraY -= 0.5;
        }
    }else{  // en caso de que estes rotando y la velocidad sea baja
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
    }
    
    if(cCar->speed>=0)
        CalculatePositionIA(cCar,cTransformable, d);
    else
        CalculatePositionReverseIA(cCar,cTransformable, d);
    CalculatePositionCameraIA(cTransformable,cTransformableCam,cCamera);

}

//Entra cuando se presiona la D
void TurnRightIA(Data d){
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
        if(cCamera->rotExtraY < 15){
            cCamera->rotExtraY += 0.5;
        }
    }else{
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
    }
    
    if(cCar->speed>=0)
        CalculatePositionIA(cCar,cTransformable, d);
    else
        CalculatePositionReverseIA(cCar,cTransformable, d);
    CalculatePositionCameraIA(cTransformable,cTransformableCam,cCamera);
}

//Aqui entra cuando no se esta presionando ni I ni O
void NotAcceleratingOrDeceleratingIA(Data d){
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
        CalculatePositionIA(cCar,cTransformable, d);
    else
        CalculatePositionReverseIA(cCar,cTransformable, d);
    CalculatePositionCameraIA(cTransformable,cTransformableCam,cCamera);
}

//Aqui entra cuando no se esta presionando ni A ni D
void NotTurningIA(Data d){
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
        CalculatePositionIA(cCar,cTransformable, d);
    else
        CalculatePositionReverseIA(cCar,cTransformable, d);
    CalculatePositionCameraIA(cTransformable,cTransformableCam,cCamera);
}

//Calcula la posicion del coche (duda con las formulas preguntar a Jose)
void CalculatePositionIA(CCar* cCar, CTransformable* cTransformable, Data d){
    float angleRotation = (cTransformable->rotation.y * PI) /180.0;
    
    //Modificamos la posicion en X y Z en funcion del angulo
    cTransformable->position.x += sin(angleRotation) * cCar->speed * d.deltaTime;
    cTransformable->position.z += cos(angleRotation) * cCar->speed * d.deltaTime;

    //Si tiene rotacion, rotamos el coche
    if(cCar->wheelRotation != 0){
        cTransformable->rotation.y += cCar->wheelRotation * 0.20;
    }
}


//Calcula la posicion del coche (duda con las formulas preguntar a Jose)
void CalculatePositionReverseIA(CCar* cCar, CTransformable* cTransformable, Data d){
    float angleRotation = (cTransformable->rotation.y * PI) /180.0;
    
    //Modificamos la posicion en X y Z en funcion del angulo
    cTransformable->position.x += sin(angleRotation) * cCar->speed * d.deltaTime;
    cTransformable->position.z += cos(angleRotation) * cCar->speed * d.deltaTime;

    //Si tiene rotacion, rotamos el coche
    if(cCar->wheelRotation != 0){
        cTransformable->rotation.y -= cCar->wheelRotation * 0.20;
    }
}

//Calcula la posicion de la camara (duda con las formulas preguntar a Jose)
void CalculatePositionCameraIA(CTransformable* cTransformableCar,CTransformable* cTransformableCamera, CCamera* cCamera){
    cTransformableCamera->position.y = cTransformableCar->position.y + 20;
    cTransformableCamera->position.x = (cTransformableCar->position.x - 40 * sin(((cTransformableCar->rotation.y - cCamera->rotExtraY)*PI)/180.0));
    cTransformableCamera->position.z = (cTransformableCar->position.z - 40 * cos(((cTransformableCar->rotation.y - cCamera->rotExtraY)*PI)/180.0));
}