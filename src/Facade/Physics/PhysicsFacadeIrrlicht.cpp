#include "PhysicsFacadeIrrlicht.h"

#include "../../Components/CBoundingSphere.h"
#include "../../Components/CCamera.h"
#include "../../Components/CCar.h"
#include "../../Components/CId.h"
#include "../../Components/CMesh.h"
#include "../../Components/CTexture.h"
#include "../../Components/CTransformable.h"
#include "../../Components/CType.h"
#include "../../Components/Component.h"
#include "../../Constants.h"

PhysicsFacadeIrrlicht::PhysicsFacadeIrrlicht() {
    auto renderFacadeManager = RenderFacadeManager::GetInstance();
    auto renderEngine = renderFacadeManager->GetRenderFacade();
    renderEngineIrrlicht = static_cast<RenderFacadeIrrlicht*>(renderEngine);
    smgr = renderEngineIrrlicht->GetSceneManager();
}

PhysicsFacadeIrrlicht::~PhysicsFacadeIrrlicht() {
}

//TODO: Aqui debera recibir el array de entidades (o acceder mediante singleton)
void PhysicsFacadeIrrlicht::UpdateCar(Entity* car, Entity* cam) {
    //Guardamos en variables los componentes
    auto cTransformable = static_cast<CTransformable*>(car->GetComponent(CompType::TransformableComp).get());
    auto cId = static_cast<CId*>(car->GetComponent(CompType::IdComp).get());

    //Actualizamos el valor en la estructura de irrlicht
    // Cogemos el nodo de irrlicht con el ID igual al que le hemos pasado
    scene::ISceneNode* node = smgr->getSceneNodeFromId(cId->id);

    //Actualiza la posicion del objeto de irrlicht
    node->setPosition(core::vector3df(cTransformable->position.x, cTransformable->position.y, cTransformable->position.z));

    //Actualiza la rotacion del objeto de irrlicht
    node->setRotation(core::vector3df(cTransformable->rotation.x, cTransformable->rotation.y, cTransformable->rotation.z));

    //Actualiza el escalado del objeto de irrlicht
    node->setScale(core::vector3df(cTransformable->scale.x, cTransformable->scale.y, cTransformable->scale.z));
    bool hasSphere = car->HasComponent(CompType::CompBoundingSphere);
    if (hasSphere && Constants::DEBUG_SHOW_SPHERES) {
        auto cSphere = static_cast<CBoundingSphere*>(car->GetComponent(CompType::CompBoundingSphere).get());
        scene::ISceneNode* nodeSphere = smgr->getSceneNodeFromId(cId->id + Component::ID_DIFFERENCE);
        nodeSphere->setPosition(core::vector3df(cSphere->center.x, cSphere->center.y, cSphere->center.z));
        nodeSphere->setVisible(RenderFacadeIrrlicht::showDebug);
    }
}

void PhysicsFacadeIrrlicht::UpdateCam(Entity* cam) {
    //Cogemos los componentes de la camara
    auto cTransformable = static_cast<CTransformable*>(cam->GetComponent(CompType::TransformableComp).get());

    //Cogemos la posicion de nuestro coche
    //TODO: cambiar ese 0 por el id de CarManager
    auto camera1 = renderEngineIrrlicht->GetCamera1();
    core::vector3df targetPosition = smgr->getSceneNodeFromId(0)->getPosition();
    targetPosition.Y += 17;
    camera1->setTarget(targetPosition);

    camera1->setPosition(core::vector3df(cTransformable->position.x, cTransformable->position.y, cTransformable->position.z));
}

//TODO:: Cambiar para que rediba todo el Manager
void PhysicsFacadeIrrlicht::UpdateCarAI(Entity* car) {
    //Guardamos en variables los componentes
    auto cTransformable = static_cast<CTransformable*>(car->GetComponent(CompType::TransformableComp).get());
    auto cId = static_cast<CId*>(car->GetComponent(CompType::IdComp).get());

    //Actualizamos el valor en la estructura de irrlicht
    // Cogemos el nodo de irrlicht con el ID igual al que le hemos pasado
    scene::ISceneNode* node = smgr->getSceneNodeFromId(cId->id);

    //Actualiza la posicion del objeto de irrlicht
    node->setPosition(core::vector3df(cTransformable->position.x, cTransformable->position.y, cTransformable->position.z));

    //Actualiza la rotacion del objeto de irrlicht
    node->setRotation(core::vector3df(cTransformable->rotation.x, cTransformable->rotation.y, cTransformable->rotation.z));

    //Actualiza el escalado del objeto de irrlicht
    node->setScale(core::vector3df(cTransformable->scale.x, cTransformable->scale.y, cTransformable->scale.z));
    bool hasSphere = car->HasComponent(CompType::CompBoundingSphere);
    if (hasSphere && Constants::DEBUG_SHOW_SPHERES) {
        auto cSphere = static_cast<CBoundingSphere*>(car->GetComponent(CompType::CompBoundingSphere).get());
        scene::ISceneNode* nodeSphere = smgr->getSceneNodeFromId(cId->id + Component::ID_DIFFERENCE);
        nodeSphere->setPosition(core::vector3df(cSphere->center.x, cSphere->center.y, cSphere->center.z));
        nodeSphere->setVisible(RenderFacadeIrrlicht::showDebug);
        //nodeSphere->setRotation(core::vector3df(cTransformable->rotation.x, cTransformable->rotation.y, cTransformable->rotation.z));
        //nodeSphere->setScale(core::vector3df(cTransformable->scale.x, cTransformable->scale.y, cTransformable->scale.z));
    }
}

//TODO:: Cambiar para que rediba todo el Manager
void PhysicsFacadeIrrlicht::UpdatePowerUps(Entity* powerUp) {
    //Guardamos en variables los componentes
    auto cTransformable = static_cast<CTransformable*>(powerUp->GetComponent(CompType::TransformableComp).get());
    auto cId = static_cast<CId*>(powerUp->GetComponent(CompType::IdComp).get());

    //Actualizamos el valor en la estructura de irrlicht
    // Cogemos el nodo de irrlicht con el ID igual al que le hemos pasado
    scene::ISceneNode* node = smgr->getSceneNodeFromId(cId->id);

    //Actualiza la posicion del objeto de irrlicht
    node->setPosition(core::vector3df(cTransformable->position.x, cTransformable->position.y, cTransformable->position.z));

    //Actualiza la rotacion del objeto de irrlicht
    node->setRotation(core::vector3df(cTransformable->rotation.x, cTransformable->rotation.y, cTransformable->rotation.z));

    //Actualiza el escalado del objeto de irrlicht
    node->setScale(core::vector3df(cTransformable->scale.x, cTransformable->scale.y, cTransformable->scale.z));
}