#include "SystemLoD.h"
#include "../Components/CCamera.h"
#include "../Components/CMesh.h"
#include "../Entities/Camera.h"
#include "../Entities/Entity.h"
#include "../../include/glm/glm.hpp"



void SetMeshAccordingToDistance(float distanceToCam, CMesh *cMesh) {
    if (distanceToCam < cMesh->distanceNear) {
        cMesh->activeMesh = cMesh->mesh[0];
    } else if(cMesh->mesh.size() >= 2 && distanceToCam < cMesh->distanceMedium) {
        cMesh->activeMesh = cMesh->mesh[1];
        cout << "Hemos puesto un mesh medium porque la distancia era " << distanceToCam << endl;
    } else if(cMesh->mesh.size() >= 3 && distanceToCam > cMesh->distanceMedium) {
        cMesh->activeMesh = cMesh->mesh[2];
    }
}

void SystemLoD::Update(vector<shared_ptr<Entity>> entities, Camera *cam) {
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