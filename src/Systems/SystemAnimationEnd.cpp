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
    vec3 auxPosition(0.f, 0.f, 0.f);
    auxPosition.x = sin(glm::radians(rotation));
    auxPosition.z = cos(glm::radians(rotation));
    // cout << "rot[" << rotation << "] Current auxPosition -> " << auxPosition.x << "," << auxPosition.y << "," << auxPosition.z << endl;


    auxPosition *= 30;
    cTransforCam->position += auxPosition;
    rotation += ROTATION_SPEED;
    // rotation = rotation % 360;
    // cout << "Current auxPosition -> " << auxPosition.x << "," << auxPosition.y << "," << auxPosition.z << endl;
    // cout << "Current rotation " << rotation << endl;
    // cout << "Current cam pos -> " << currentPos.x << "," << currentPos.y << "," << currentPos.z << endl;
}

void SystemAnimationEnd::ResetTimer() {
    timeStartAnim = Utils::getMillisSinceEpoch();
}

void SystemAnimationEnd::SetWinner(Car *winner_) {
    winner = winner_;
    auto cTransformable = static_cast<CTransformable *>(winner->GetComponent(CompType::TransformableComp).get());
    cam->setTarget(cTransformable->position);
}