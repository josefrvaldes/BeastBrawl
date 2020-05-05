#include "PhysicsFacadeClover.h"

#include <Components/CId.h>
#include <Components/CWheel.h>
#include <Components/CCar.h>
#include <Components/CBoundingChassis.h>
#include <Components/CBoundingSphere.h>
#include <Systems/Utils.h>
#include <Constants.h>

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

    // Cogemos el nodo de irrlicht con el ID igual al que le hemos pasado
    auto node = device->GetNodeByID(cId->id); 

    //cout << cTransformable->position.x << " | " << cTransformable->position.y << " | " << cTransformable->position.z << endl;
    //Actualiza la posicion del objeto de irrlicht
    node->SetTranslation(glm::vec3(cTransformable->position.x, cTransformable->position.y, -cTransformable->position.z));

    //Actualiza la rotacion del objeto de irrlicht
    node->SetRotation(glm::vec3(cTransformable->rotation.x, Utils::IrrlichtAngleToOpenGL(cTransformable->rotation.y), cTransformable->rotation.z));

    //Actualiza el escalado del objeto de irrlicht
    node->SetScalation(glm::vec3(cTransformable->scale.x, cTransformable->scale.y, cTransformable->scale.z));

    
    // vamos a ver si tiene CBoundingChassis
    bool hasChassis = car->HasComponent(CompType::CompBoundingChassis); 
    if (hasChassis && Constants::DEBUG_SHOW_CHASSIS) {
        auto cChassis = static_cast<CBoundingChassis *>(car->GetComponent(CompType::CompBoundingChassis).get());
        auto cSphere1 = cChassis->sphereBehind;
        auto nodeSphere1 = device->GetNodeByID(cId->id + Component::ID_DIFFERENCE);
        nodeSphere1->SetTranslation(glm::vec3(cSphere1->center.x, cSphere1->center.y, -cSphere1->center.z));
        nodeSphere1->SetVisible(RenderFacadeClover::showDebug);
        auto cSphere2 = cChassis->sphereFront;
        auto nodeSphere2 = device->GetNodeByID(cId->id + Component::ID_DIFFERENCE + Component::ID_DIFFERENCE);
        nodeSphere2->SetTranslation(glm::vec3(cSphere2->center.x, cSphere2->center.y, -cSphere2->center.z));

        nodeSphere2->SetVisible(RenderFacadeClover::showDebug);

    }

    if(car->HasComponent(CompType::WheelComp) && car->HasComponent(CompType::CarComp)){
        auto cWheel = static_cast<CWheel*>(car->GetComponent(CompType::WheelComp).get());
        auto cCar = static_cast<CCar*>(car->GetComponent(CompType::CarComp).get());
        auto wheel1 = device->GetNodeByID(cWheel->IdWheelTopLeft);
        auto wheel2 = device->GetNodeByID(cWheel->IdWheelTopRight);
        auto wheel3 = device->GetNodeByID(cWheel->IdWheelBottomLeft);
        auto wheel4 = device->GetNodeByID(cWheel->IdWheelBottomRight);

        cWheel->rotationTopLeft.z += cCar->speed * Constants::DELTA_TIME;
        cWheel->rotationTopRight.z += cCar->speed * Constants::DELTA_TIME;
        cWheel->rotationBottomLeft.z += cCar->speed * Constants::DELTA_TIME;
        cWheel->rotationBottomRight.z += cCar->speed * Constants::DELTA_TIME;

        // wheel1->SetRotation(glm::vec3(cWheel->rotationTopLeft.x, Utils::IrrlichtAngleToOpenGL(cWheel->rotationTopLeft.y),cWheel->rotationTopLeft.z));
        // wheel2->SetRotation(glm::vec3(cWheel->rotationTopRight.x, Utils::IrrlichtAngleToOpenGL(cWheel->rotationTopRight.y),cWheel->rotationTopRight.z));
        // wheel3->SetRotation(glm::vec3(cWheel->rotationBottomLeft.x, Utils::IrrlichtAngleToOpenGL(cWheel->rotationBottomLeft.y),cWheel->rotationBottomLeft.z));
        // wheel4->SetRotation(glm::vec3(cWheel->rotationBottomRight.x, Utils::IrrlichtAngleToOpenGL(cWheel->rotationBottomRight.y),cWheel->rotationBottomRight.z));
    }
}

//@deprecated
void PhysicsFacadeClover::UpdateCam(Entity* cam) {
    //Cogemos los componentes de la camara 
    auto cTransformable = static_cast<CTransformable*>(cam->GetComponent(CompType::TransformableComp).get());
    auto cId = static_cast<CId*>(cam->GetComponent(CompType::IdComp).get());

    //Cogemos la posicion de nuestro coche
    //TODO: cambiar ese 0 por el id de CarManager
    auto camera = device->GetNodeByID(cId->id);
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
    auto node = device->GetNodeByID(cId->id);

    //Actualiza la posicion del objeto de irrlicht
    node->SetTranslation(glm::vec3(cTransformable->position.x, cTransformable->position.y, -cTransformable->position.z));

    //Actualiza la rotacion del objeto de irrlicht
    node->SetRotation(glm::vec3(cTransformable->rotation.x, Utils::IrrlichtAngleToOpenGL(cTransformable->rotation.y), cTransformable->rotation.z));

    //Actualiza el escalado del objeto de irrlicht
    node->SetScalation(glm::vec3(cTransformable->scale.x, cTransformable->scale.y, cTransformable->scale.z));

   
   
    
    // vamos a ver si tiene CBoundingChassis
    bool hasChassis = entity->HasComponent(CompType::CompBoundingChassis);
    if (hasChassis && Constants::DEBUG_SHOW_CHASSIS) {
        auto cChassis = static_cast<CBoundingChassis *>(entity->GetComponent(CompType::CompBoundingChassis).get());
        auto cSphere1 = cChassis->sphereBehind;
        auto nodeSphere1 = device->GetNodeByID(cId->id + Component::ID_DIFFERENCE);
        nodeSphere1->SetTranslation(glm::vec3(cSphere1->center.x, cSphere1->center.y, -cSphere1->center.z));
        nodeSphere1->SetVisible(RenderFacadeClover::showDebug);
        auto cSphere2 = cChassis->sphereFront;
        auto nodeSphere2 = device->GetNodeByID(cId->id + Component::ID_DIFFERENCE + Component::ID_DIFFERENCE );
        nodeSphere2->SetTranslation(glm::vec3(cSphere2->center.x, cSphere2->center.y, -cSphere2->center.z));
        nodeSphere2->SetVisible(RenderFacadeClover::showDebug);

    }

}

//TODO:: Cambiar para que rediba todo el Manager
void PhysicsFacadeClover::UpdatePowerUps(Entity* powerUp) {
    //Guardamos en variables los componentes
    auto cTransformable = static_cast<CTransformable*>(powerUp->GetComponent(CompType::TransformableComp).get());
    auto cId = static_cast<CId*>(powerUp->GetComponent(CompType::IdComp).get());

    //Actualizamos el valor en la estructura de irrlicht
    // Cogemos el nodo de irrlicht con el ID igual al que le hemos pasado
    auto node = device->GetNodeByID(cId->id);

    //Actualiza la posicion del objeto de irrlicht
    node->SetTranslation(glm::vec3(cTransformable->position.x, cTransformable->position.y, -cTransformable->position.z));

    //Actualiza la rotacion del objeto de irrlicht
    node->SetRotation(glm::vec3(cTransformable->rotation.x, Utils::IrrlichtAngleToOpenGL(cTransformable->rotation.y), cTransformable->rotation.z));

    //Actualiza el escalado del objeto de irrlicht
    node->SetScalation(glm::vec3(cTransformable->scale.x, cTransformable->scale.y, cTransformable->scale.z));
}


PhysicsFacadeClover::~PhysicsFacadeClover() {
}