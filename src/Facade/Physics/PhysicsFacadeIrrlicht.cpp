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


PhysicsFacadeIrrlicht::PhysicsFacadeIrrlicht(){

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
    //TODO: cambiar ese 0 por el id de CarManager
    auto camera1 = renderEngineIrrlicht->GetCamera1();
	core::vector3df targetPosition  = smgr->getSceneNodeFromId(0)->getPosition();
    targetPosition.Y += 17;
    camera1->setTarget(targetPosition);

	camera1->setPosition(core::vector3df(cTransformable->posX, cTransformable->posY, cTransformable->posZ));
}



PhysicsFacadeIrrlicht::~PhysicsFacadeIrrlicht(){

}

