/**
 * Beast Brawl
 * Game created as a final project of the Multimedia Engineering Degree in the University of Alicante.
 * Made by Clover Games Studio, with members 
 * Carlos de la Fuente Torres delafuentetorresc@gmail.com,
 * Antonio Jose Martinez Garcia https://www.linkedin.com/in/antonio-jose-martinez-garcia/,
 * Jesús Mas Carretero jmasc03@gmail.com, 
 * Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/, 
 * Rubén Rubio Martínez https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/, 
 * and Jose Valdés Sirvent https://www.linkedin.com/in/jose-f-valdés-sirvent-6058b5a5/ github -> josefrvaldes
 * 
 * 
 * @author Clover Games Studio
 * 
 */
 
 
 #include "PhysicsFacadeClover.h"

#include <Components/CId.h>
#include <Components/CWheel.h>
#include <Components/CCar.h>
#include <Components/CBoundingChassis.h>
#include <Components/CBoundingSphere.h>
#include <Components/CParentNode.h>
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

        cWheel->rotationTopLeft.z += cCar->speed * Constants::DELTA_TIME  * 6;
        cWheel->rotationTopRight.z += cCar->speed * Constants::DELTA_TIME * 6;
        cWheel->rotationBottomLeft.z += cCar->speed * Constants::DELTA_TIME * 6;
        cWheel->rotationBottomRight.z += cCar->speed * Constants::DELTA_TIME * 6;

        cWheel->rotationTopLeft.y = cCar->wheelRotation * 2;
        cWheel->rotationTopRight.y = cCar->wheelRotation * 2;

        wheel1->SetRotation(glm::vec3(cWheel->rotationTopLeft.x, Utils::IrrlichtAngleToOpenGL(cWheel->rotationTopLeft.y),cWheel->rotationTopLeft.z));
        wheel2->SetRotation(glm::vec3(cWheel->rotationTopRight.x, Utils::IrrlichtAngleToOpenGL(cWheel->rotationTopRight.y),cWheel->rotationTopRight.z));
        wheel3->SetRotation(glm::vec3(cWheel->rotationBottomLeft.x, Utils::IrrlichtAngleToOpenGL(cWheel->rotationBottomLeft.y),cWheel->rotationBottomLeft.z));
        wheel4->SetRotation(glm::vec3(cWheel->rotationBottomRight.x, Utils::IrrlichtAngleToOpenGL(cWheel->rotationBottomRight.y),cWheel->rotationBottomRight.z));
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

    if(entity->HasComponent(CompType::WheelComp) && entity->HasComponent(CompType::CarComp)){
        auto cWheel = static_cast<CWheel*>(entity->GetComponent(CompType::WheelComp).get());
        auto cCar = static_cast<CCar*>(entity->GetComponent(CompType::CarComp).get());
        auto wheel1 = device->GetNodeByID(cWheel->IdWheelTopLeft);
        auto wheel2 = device->GetNodeByID(cWheel->IdWheelTopRight);
        auto wheel3 = device->GetNodeByID(cWheel->IdWheelBottomLeft);
        auto wheel4 = device->GetNodeByID(cWheel->IdWheelBottomRight);

        cWheel->rotationTopLeft.z += cCar->speed * Constants::DELTA_TIME * 6;
        cWheel->rotationTopRight.z += cCar->speed * Constants::DELTA_TIME * 6;
        cWheel->rotationBottomLeft.z += cCar->speed * Constants::DELTA_TIME * 6;
        cWheel->rotationBottomRight.z += cCar->speed * Constants::DELTA_TIME * 6;

        cWheel->rotationTopLeft.y = cCar->wheelRotation * 2;
        cWheel->rotationTopRight.y = cCar->wheelRotation * 2;

        wheel1->SetRotation(glm::vec3(cWheel->rotationTopLeft.x, Utils::IrrlichtAngleToOpenGL(cWheel->rotationTopLeft.y),cWheel->rotationTopLeft.z));
        wheel2->SetRotation(glm::vec3(cWheel->rotationTopRight.x, Utils::IrrlichtAngleToOpenGL(cWheel->rotationTopRight.y),cWheel->rotationTopRight.z));
        wheel3->SetRotation(glm::vec3(cWheel->rotationBottomLeft.x, Utils::IrrlichtAngleToOpenGL(cWheel->rotationBottomLeft.y),cWheel->rotationBottomLeft.z));
        wheel4->SetRotation(glm::vec3(cWheel->rotationBottomRight.x, Utils::IrrlichtAngleToOpenGL(cWheel->rotationBottomRight.y),cWheel->rotationBottomRight.z));
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


void PhysicsFacadeClover::UpdateShields(const vector<shared_ptr<Entity>> &cars, const vector<shared_ptr<Entity>> &shields) {
    //cout << "------------------- ACTUALIZANDO ESCUDOS" << endl;
    for (const auto &car : cars) {
        auto cTransformable = static_cast<CTransformable*>(car->GetComponent(CompType::TransformableComp).get());
        auto cId = static_cast<CId*>(car->GetComponent(CompType::IdComp).get());

        if (cTransformable && cId) {

            // Busco cual es mi coche
            for (auto& shield: shields) {
                auto cParentNode = static_cast<CParentNode*>(shield->GetComponent(CompType::ParentNodeComp).get());
                    auto cTransformableShield = static_cast<CTransformable*>(shield->GetComponent(CompType::TransformableComp).get());
                    auto cIdShield = static_cast<CId*>(shield->GetComponent(CompType::IdComp).get());

                if (cTransformableShield && cIdShield && cParentNode && cParentNode->idParentNode == cId->id) {
                    //cout << "ESTE ES MI ESCUDO: [Coche] " << cId->id << " [Padre escudo] " << cParentNode->idParentNode << " [Escudo] " << cIdShield->id << endl;  
               
                    cTransformableShield->position = cTransformable->position;
                    cTransformableShield->rotation = cTransformable->rotation;

                    auto node = device->GetNodeByID(cIdShield->id);
                    node->SetTranslation(glm::vec3(cTransformableShield->position.x, cTransformableShield->position.y, -cTransformableShield->position.z));
                    node->SetRotation(glm::vec3(cTransformableShield->rotation.x, Utils::IrrlichtAngleToOpenGL(cTransformableShield->rotation.y), cTransformableShield->rotation.z));
               
                }

            }
        }

    }

}

PhysicsFacadeClover::~PhysicsFacadeClover() {
}