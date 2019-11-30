#include "PhysicsFacadeIrrlicht.h"


#include "../../Components/CPosition.h"
#include "../../Components/CType.h"
#include "../../Components/CId.h"
#include "../../Components/CTexture.h"
#include "../../Components/CMesh.h"
#include "../../Components/CTransformable.h"
#include "../../Components/CCamera.h"
#include "../../Components/Component.h"
#include "../../Components/CCar.h"

#include <math.h>
#define PI 3.141592

void CalculatePosition(CCar* cCar, CTransformable* cTransformable, Data d);
void CalculatePositionCamera(CTransformable* cTransformableCar,CTransformable* cTransformableCamera, CCamera* cCamera);

//TODO: Cambiar en los punteros a funciones en vez de ir pasandole datos por Event cogerlos del EntityManager
// PUNTEROS A FUNCIONES
void PressW(Data d);
void PressA(Data d);
void PressD(Data d);
void PressS(Data d);
void NoWSPress(Data d);
void NoADPress(Data d);

PhysicsFacadeIrrlicht::PhysicsFacadeIrrlicht(){
    eventManager = EventManager::GetInstance();

    //Se suscriben los listeners
    eventManager->Suscribe(Listener {EventType::PRESS_W,PressW,"pressW"});
    eventManager->Suscribe(Listener {EventType::PRESS_A,PressA,"pressA"});
    eventManager->Suscribe(Listener {EventType::PRESS_D,PressD,"pressD"});
    eventManager->Suscribe(Listener {EventType::PRESS_S,PressS,"pressS"});
    eventManager->Suscribe(Listener {EventType::NO_W_S_PRESS,NoWSPress,"noWSPress"});
    eventManager->Suscribe(Listener {EventType::NO_A_D_PRESS,NoADPress,"noADPress"});

    auto renderFacadeManager = RenderFacadeManager::GetInstance();
    auto renderEngine = renderFacadeManager->GetRenderFacade();
    renderEngineIrrlicht = static_cast<RenderFacadeIrrlicht*>(renderEngine);
    smgr = renderEngineIrrlicht->GetSceneManager();

}


//TODO: Aqui debera recibir el array de entidades (o acceder mediante singleton)
void PhysicsFacadeIrrlicht::Update(Entity* car, Entity* cam){
    

    auto smgr = renderEngineIrrlicht->GetSceneManager();

    //Guardamos en variables los componentes
    auto components = car->GetComponents();
    auto mapTransform = components.find(CompType::TransformableComp);
	auto cTransformable = static_cast<CTransformable*>(mapTransform->second);
    auto mapId = components.find(CompType::IdComp);
	auto cId = static_cast<CId*>(mapId->second);

    //Actualizamos el valor en la estructura de irrlicht
    // Cogemos el nodo de irrlicht con el ID igual al que le hemos pasado
	scene::ISceneNode* node = smgr->getSceneNodeFromId(cId->id);

	//Actualiza la posicion del objeto de irrlicht
	node->setPosition(core::vector3df(cTransformable->posX, cTransformable->posY, cTransformable->posZ));

	//Actualiza la rotacion del objeto de irrlicht
	node->setRotation(core::vector3df(cTransformable->rotX, cTransformable->rotY, cTransformable->rotZ));

	//Actualiza el escalado del objeto de irrlicht
	node->setScale(core::vector3df(cTransformable->scaleX, cTransformable->scaleY, cTransformable->scaleZ));


    //Actualizamos la camara
    UpdateCam(cam);
    

}

void PhysicsFacadeIrrlicht::UpdateCam(Entity* cam){
    //Cogemos los componentes de la camara
	auto components = cam->GetComponents();
	auto mapTransformable = components.find(CompType::TransformableComp);
	auto cTransformable = static_cast<CTransformable*>(mapTransformable->second);

	//Cogemos la posicion de nuestro coche
    auto camera1 = renderEngineIrrlicht->GetCamera1();
	core::vector3df targetPosition  = smgr->getSceneNodeFromId(0)->getPosition();
    targetPosition.Y += 17;
    camera1->setTarget(targetPosition);

	camera1->setPosition(core::vector3df(cTransformable->posX, cTransformable->posY, cTransformable->posZ));
}



PhysicsFacadeIrrlicht::~PhysicsFacadeIrrlicht(){

}

//PUNTEROS A FUNCIONES

//Entra cuando se presiona la W
void PressW(Data d){

    //Guardamos en variables los componentes
    auto components = d.gameObject->GetComponents();
    auto mapTransform = components.find(CompType::TransformableComp);
	auto cTransformable = static_cast<CTransformable*>(mapTransform->second);
    
    auto mapCar = components.find(CompType::CarComp);
    auto cCar        = static_cast<CCar*>(mapCar->second);

    //Aumentamos la velocidad
    cCar->speed += cCar->acceleration;
    if(cCar->speed >= cCar->maxSpeed){
        cCar->speed = cCar->maxSpeed;
    }

    CalculatePosition(cCar,cTransformable, d);
   
}

//Entra cuando se presiona la S
void PressS(Data d){
    //Guardamos en variables los componentes
    auto components = d.gameObject->GetComponents();
    auto mapTransform = components.find(CompType::TransformableComp);
	auto cTransformable = static_cast<CTransformable*>(mapTransform->second);
    auto mapCar = components.find(CompType::CarComp);
    auto cCar        = static_cast<CCar*>(mapCar->second);

    //Reducimos la velocidad
    cCar->speed -= cCar->acceleration;
    if(cCar->speed < 0){
        cCar->speed = 0;
    }
    CalculatePosition(cCar,cTransformable, d);
}

//Entra cuando se presiona la A
void PressA(Data d){

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


    }
    CalculatePosition(cCar,cTransformable, d);
    CalculatePositionCamera(cTransformable,cTransformableCam,cCamera);

}

//Entra cuando se presiona la D
void PressD(Data d){

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
    }
    CalculatePosition(cCar,cTransformable, d);
    CalculatePositionCamera(cTransformable,cTransformableCam,cCamera);

}

//Aqui entra cuando no se esta presionando ni W ni S
void NoWSPress(Data d){

    //Guardamos en variables los componentes
    auto components = d.gameObject->GetComponents();
    auto mapTransform = components.find(CompType::TransformableComp);
	auto cTransformable = static_cast<CTransformable*>(mapTransform->second);
    
    auto mapCar = components.find(CompType::CarComp);
    auto cCar        = static_cast<CCar*>(mapCar->second);

    if(cCar->speed > 0){
        cCar->speed -= cCar->friction;
        if(cCar->speed < 0){
            cCar->speed = 0;
        }
    }

    CalculatePosition(cCar,cTransformable, d);
    

}

//Aqui entra cuando no se esta presionando ni A ni D
void NoADPress(Data d){

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

    CalculatePosition(cCar,cTransformable, d);
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

//Calcula la posicion de la camara (duda con las formulas preguntar a Jose)
void CalculatePositionCamera(CTransformable* cTransformableCar,CTransformable* cTransformableCamera, CCamera* cCamera){
    cTransformableCamera->posY = cTransformableCar->posY + 20;
    cTransformableCamera->posX = (cTransformableCar->posX - 40 * sin(((cTransformableCar->rotY - cCamera->rotExtraY)*PI)/180.0));
    cTransformableCamera->posZ = (cTransformableCar->posZ - 40 * cos(((cTransformableCar->rotY - cCamera->rotExtraY)*PI)/180.0));
}
