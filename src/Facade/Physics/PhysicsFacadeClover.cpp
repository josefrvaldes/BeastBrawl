#include "PhysicsFacadeClover.h"

#include <Components/CId.h>
#include <Components/CBoundingChassis.h>
#include <Components/CBoundingSphere.h>
#include <Systems/Utils.h>

PhysicsFacadeClover::PhysicsFacadeClover() {
    
    auto renderFacade = static_cast<RenderFacadeClover*>(RenderFacadeManager::GetInstance()->GetRenderFacade());
    device = renderFacade->GetDevice();
    smgr = device->GetSceneManager();
}



//TODO: Aqui debera recibir el array de entidades (o acceder mediante singleton)
void PhysicsFacadeClover::UpdateCar(Entity* car, Entity* cam) {
    //Guardamos en variables los componentes
    auto cTransformable = static_cast<CTransformable*>(car->GetComponent(CompType::TransformableComp).get());
    auto cId = static_cast<CId*>(car->GetComponent(CompType::IdComp).get());

    auto irrAngle = cTransformable->rotation.y;
    auto openglAngle = Utils::IrrlichtAngleToOpenGL(130.0);
    //Actualizamos el valor en la estructura de irrlicht
    // Cogemos el nodo de irrlicht con el ID igual al que le hemos pasado
    auto node = smgr->GetNodeByID(cId->id);

    //Actualiza la posicion del objeto de irrlicht
    node->SetTranslation(glm::vec3(cTransformable->position.x, cTransformable->position.y, -cTransformable->position.z));

    //Actualiza la rotacion del objeto de irrlicht
    node->SetRotation(glm::vec3(cTransformable->rotation.x, Utils::IrrlichtAngleToOpenGL(cTransformable->rotation.y), cTransformable->rotation.z));

    //Actualiza el escalado del objeto de irrlicht
    node->SetScalation(glm::vec3(cTransformable->scale.x, cTransformable->scale.y, cTransformable->scale.z));

    
    // vamos a ver si tiene CBoundingChassis
    // bool hasChassis = car->HasComponent(CompType::CompBoundingChassis); 
    // if (hasChassis && Constants::DEBUG_SHOW_CHASSIS) {
    //     auto cChassis = static_cast<CBoundingChassis *>(car->GetComponent(CompType::CompBoundingChassis).get());
    //     auto cSphere1 = cChassis->sphereBehind;
    //     auto nodeSphere1 = smgr->GetNodeByID(cId->id + Component::ID_DIFFERENCE);
    //     nodeSphere1->SetTranslation(glm::vec3(cSphere1->center.x, cSphere1->center.y, cSphere1->center.z));
    //     nodeSphere1->SetVisible(RenderFacadeIrrlicht::showDebug);
    //     auto cSphere2 = cChassis->sphereFront;
    //     auto nodeSphere2 = smgr->GetNodeByID(cId->id + Component::ID_DIFFERENCE + Component::ID_DIFFERENCE);
    //     nodeSphere2->SetTranslation(glm::vec3(cSphere2->center.x, cSphere2->center.y, cSphere2->center.z));
    //     nodeSphere2->SetVisible(RenderFacadeIrrlicht::showDebug);

    // }
}

//@deprecated
void PhysicsFacadeClover::UpdateCam(Entity* cam) {
    //Cogemos los componentes de la camara 
    auto cTransformable = static_cast<CTransformable*>(cam->GetComponent(CompType::TransformableComp).get());
    auto cId = static_cast<CId*>(cam->GetComponent(CompType::IdComp).get());

    //Cogemos la posicion de nuestro coche
    //TODO: cambiar ese 0 por el id de CarManager
    auto camera = smgr->GetNodeByID(cId->id);
    auto entityCamera = static_cast<CLCamera*>(camera->GetEntity());
    glm::vec3 targetPosition = camera->GetTranslation();
    targetPosition.y += 17;
    entityCamera->SetCameraTarget(targetPosition);

    camera->SetTranslation(glm::vec3(-cTransformable->position.x, cTransformable->position.y, -cTransformable->position.z));
    camera->SetRotation(glm::vec3(-cTransformable->rotation.x, cTransformable->rotation.y, -cTransformable->rotation.z));
}

//TODO:: Cambiar para que rediba todo el Manager
void PhysicsFacadeClover::UpdateTransformable(Entity* entity) {

    //Guardamos en variables los componentes
    auto cTransformable = static_cast<CTransformable*>(entity->GetComponent(CompType::TransformableComp).get());
    auto cId = static_cast<CId*>(entity->GetComponent(CompType::IdComp).get());

    //Actualizamos el valor en la estructura de irrlicht
    // Cogemos el nodo de irrlicht con el ID igual al que le hemos pasado
    auto node = smgr->GetNodeByID(cId->id);

    //Actualiza la posicion del objeto de irrlicht
    node->SetTranslation(glm::vec3(cTransformable->position.x, cTransformable->position.y, -cTransformable->position.z));

    //Actualiza la rotacion del objeto de irrlicht
    node->SetRotation(glm::vec3(cTransformable->rotation.x, Utils::IrrlichtAngleToOpenGL(cTransformable->rotation.y), cTransformable->rotation.z));

    //Actualiza el escalado del objeto de irrlicht
    node->SetScalation(glm::vec3(cTransformable->scale.x, cTransformable->scale.y, cTransformable->scale.z));

    /*
    bool hasSphere = entity->HasComponent(CompType::CompBoundingSphere);
    if (hasSphere && Constants::DEBUG_SHOW_SPHERES) {
        auto cSphere = static_cast<CBoundingSphere*>(entity->GetComponent(CompType::CompBoundingSphere).get());
        scene::ISceneNode* nodeSphere = smgr->getSceneNodeFromId(cId->id + Component::ID_DIFFERENCE);
        nodeSphere->SetTranslation(glm::vec3(cSphere->center.x, cSphere->center.y, cSphere->center.z));
        nodeSphere->setVisible(RenderFacadeIrrlicht::showDebug);
        //nodeSphere->setRotation(glm::vec3(cTransformable->rotation.x, cTransformable->rotation.y, cTransformable->rotation.z));
        //nodeSphere->setScale(glm::vec3(cTransformable->scale.x, cTransformable->scale.y, cTransformable->scale.z));
    }
    */
   
    
    // vamos a ver si tiene CBoundingChassis
    // bool hasChassis = entity->HasComponent(CompType::CompBoundingChassis);
    // if (hasChassis && Constants::DEBUG_SHOW_CHASSIS) {
    //     auto cChassis = static_cast<CBoundingChassis *>(entity->GetComponent(CompType::CompBoundingChassis).get());
    //     auto cSphere1 = cChassis->sphereBehind;
    //     scene::ISceneNode* nodeSphere1 = smgr->getSceneNodeFromId(cId->id + Component::ID_DIFFERENCE);
    //     nodeSphere1->SetTranslation(glm::vec3(cSphere1->center.x, cSphere1->center.y, cSphere1->center.z));
    //     nodeSphere1->setVisible(RenderFacadeIrrlicht::showDebug);
    //     auto cSphere2 = cChassis->sphereFront;
    //     scene::ISceneNode* nodeSphere2 = smgr->getSceneNodeFromId(cId->id + Component::ID_DIFFERENCE + Component::ID_DIFFERENCE );
    //     nodeSphere2->SetTranslation(glm::vec3(cSphere2->center.x, cSphere2->center.y, cSphere2->center.z));
    //     nodeSphere2->setVisible(RenderFacadeIrrlicht::showDebug);

    // }

}

//TODO:: Cambiar para que rediba todo el Manager
void PhysicsFacadeClover::UpdatePowerUps(Entity* powerUp) {
    //Guardamos en variables los componentes
    auto cTransformable = static_cast<CTransformable*>(powerUp->GetComponent(CompType::TransformableComp).get());
    auto cId = static_cast<CId*>(powerUp->GetComponent(CompType::IdComp).get());

    //Actualizamos el valor en la estructura de irrlicht
    // Cogemos el nodo de irrlicht con el ID igual al que le hemos pasado
    auto node = smgr->GetNodeByID(cId->id);

    //Actualiza la posicion del objeto de irrlicht
    node->SetTranslation(glm::vec3(cTransformable->position.x, cTransformable->position.y, -cTransformable->position.z));

    //Actualiza la rotacion del objeto de irrlicht
    node->SetRotation(glm::vec3(cTransformable->rotation.x, Utils::IrrlichtAngleToOpenGL(cTransformable->rotation.y), cTransformable->rotation.z));

    //Actualiza el escalado del objeto de irrlicht
    node->SetScalation(glm::vec3(cTransformable->scale.x, cTransformable->scale.y, cTransformable->scale.z));
}


PhysicsFacadeClover::~PhysicsFacadeClover() {
}