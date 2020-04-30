#include "SystemAnimationEnd.h"

#include "../Entities/Camera.h"
#include "../Entities/Car.h"
#include "../Entities/Totem.h"
#include "../Systems/Utils.h"

SystemAnimationEnd::SystemAnimationEnd(Camera *cam_) : cam{cam_}, winner{nullptr}, timeStartAnim{Utils::getMillisSinceEpoch()} {
}

void SystemAnimationEnd::Animate() {
    MoveCam(0, 0);
}

void SystemAnimationEnd::MoveCam(int64_t interval, int64_t duration) {
    auto cTransforCar = static_cast<CTransformable *>(winner->GetComponent(CompType::TransformableComp).get());
    auto cTransforCam = static_cast<CTransformable *>(cam->GetComponent(CompType::TransformableComp).get());
    cTransforCam->position = cTransforCar->position;
    cTransforCam->position.y += 20;
    cTransforCam->position.x += sin(glm::radians(rotation)) * 30;
    cTransforCam->position.z += cos(glm::radians(rotation)) * 30;
    rotation += ROTATION_SPEED;
}

void SystemAnimationEnd::ResetTimer() {
    timeStartAnim = Utils::getMillisSinceEpoch();
}

void SystemAnimationEnd::SetWinner(Car *winner_) {
    winner = winner_;
    auto cTransformable = static_cast<CTransformable *>(winner->GetComponent(CompType::TransformableComp).get());
    cam->setTarget(cTransformable->position);
}