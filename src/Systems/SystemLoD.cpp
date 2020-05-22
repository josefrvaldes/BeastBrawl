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
 
 
 #include "SystemLoD.h"
#include "../Components/CCamera.h"
#include "../Components/CMesh.h"
#include "../Components/CAnimation.h"
#include "../Entities/Camera.h"
#include "../Entities/Entity.h"
#include "../../include/glm/glm.hpp"



void SetMeshAccordingToDistance(float distanceToCam, CMesh *cMesh) {
    cMesh->previousMesh = cMesh->activeMesh;
    if (distanceToCam < cMesh->distanceNear) {
        cMesh->activeMesh = cMesh->mesh[0];
    } else if(cMesh->mesh.size() >= 2 && distanceToCam < cMesh->distanceMedium) {
        cMesh->activeMesh = cMesh->mesh[1];
        // cout << "Hemos puesto un mesh medium porque la distancia era " << distanceToCam << endl;
    } else if(cMesh->mesh.size() >= 3 && distanceToCam > cMesh->distanceMedium) {
        cMesh->activeMesh = cMesh->mesh[2];
    }
}

void SetMeshAccordingToDistance(float distanceToCam, CAnimation *cAnimation) {
    // cAnimation->previousAnimation = cAnimation->activeAnimation.get();
    if (distanceToCam < cAnimation->distanceNear) {
        cAnimation->activeAnimation = cAnimation->GetAnimations()[0];
    } else if(cAnimation->GetAnimations().size() >= 2 && distanceToCam < cAnimation->distanceMedium) {
        cAnimation->activeAnimation = cAnimation->GetAnimations()[1];
        // cout << "Hemos puesto un mesh medium porque la distancia era " << distanceToCam << endl;
    } else if(cAnimation->GetAnimations().size() >= 3 && distanceToCam > cAnimation->distanceMedium) {
        cAnimation->activeAnimation = cAnimation->GetAnimations()[2];
    }
}

void SystemLoD::UpdateMeshes(vector<shared_ptr<Entity>> entities, Camera *cam) {
    CTransformable *cCam = static_cast<CTransformable *>(cam->GetComponent(CompType::TransformableComp).get());
    glm::vec3 camPosition = cCam->position;
    for (const auto &entity : entities) {
        CTransformable *entityTransfor = static_cast<CTransformable *>(entity->GetComponent(CompType::TransformableComp).get());
        CMesh *cMesh = static_cast<CMesh *>(entity->GetComponent(CompType::MeshComp).get());
        glm::vec3 entityPos = entityTransfor->position;
        float distanceToCam = glm::distance(camPosition, entityPos);
        SetMeshAccordingToDistance(distanceToCam, cMesh);
    }
}

void SystemLoD::UpdateAnimations(vector<shared_ptr<Entity>> entities, Camera *cam) {
    CTransformable *cCam = static_cast<CTransformable *>(cam->GetComponent(CompType::TransformableComp).get());
    glm::vec3 camPosition = cCam->position;
    for (const auto &entity : entities) {
        CTransformable *entityTransfor = static_cast<CTransformable *>(entity->GetComponent(CompType::TransformableComp).get());
        CAnimation *cAnimation = static_cast<CAnimation *>(entity->GetComponent(CompType::AnimationComp).get());
        glm::vec3 entityPos = entityTransfor->position;
        float distanceToCam = glm::distance(camPosition, entityPos);
        SetMeshAccordingToDistance(distanceToCam, cAnimation);
    }
}