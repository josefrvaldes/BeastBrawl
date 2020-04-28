#include "SystemAnimationStart.h"

#include "../Entities/Camera.h"
#include "../Entities/Car.h"
#include "../Entities/Totem.h"
#include "../Systems/Utils.h"

SystemAnimationStart::SystemAnimationStart(Camera *cam_, Totem *totem_, Car *mainCar_, Car *car_) : cam{cam_}, totem{totem_}, mainCar{mainCar_}, car{car_}, timeStartAnim{Utils::getMillisSinceEpoch()} {
}

bool SystemAnimationStart::Animate() {
    switch (currentIndexAnimation) {
        case 0:
            AnimatePanMap();
            break;
        case 1:
            AnimateTotem();
            break;
        case 2:
            AnimateRandomCar();
            break;

        default:
            return true;
            break;
    }
    return false;
}

void SystemAnimationStart::MoveCam(int64_t interval, int64_t duration) {
    // cout << "interval -> " << interval << endl;
    float percentTick = std::min(1.0f, (static_cast<float>(interval) / duration));
    // cout << "percentTick -> " << percentTick << endl;
    vec3 currentPos = mix(currentStartPosition, currentEndPosition, percentTick);
    auto cTransfor = static_cast<CTransformable *>(cam->GetComponent(CompType::TransformableComp).get());
    cTransfor->position = currentPos;
    // cout << "Current cam pos -> " << currentPos.x << "," << currentPos.y << "," << currentPos.z << endl;
}

void SystemAnimationStart::AnimatePanMap() {
    int64_t now = Utils::getMillisSinceEpoch();
    int64_t interval = now - timeStartAnim;
    int64_t currentDuration = durations[currentIndexAnimation];
    if (interval < currentDuration) {
        MoveCam(interval, currentDuration);
    } else {
        auto cTransformableTotem = static_cast<CTransformable*>(totem->GetComponent(CompType::TransformableComp).get());
        cam->setTarget(cTransformableTotem->position);
        CalculatePosStartEndTotem();
        ResetTimer();
        currentIndexAnimation++;
    }
};

void SystemAnimationStart::CalculatePosStartEndTotem() {
    auto cTransformableTotem = static_cast<CTransformable*>(totem->GetComponent(CompType::TransformableComp).get());
    currentStartPosition = currentEndPosition = cTransformableTotem->position;
    currentStartPosition.x += 50;
    currentEndPosition.z += 50;
}

void SystemAnimationStart::CalculatePosStartEndRandomCar() {
    auto cTransformableTotem = static_cast<CTransformable*>(car->GetComponent(CompType::TransformableComp).get());
    currentStartPosition = currentEndPosition = cTransformableTotem->position;
    currentStartPosition.z += 50;
    currentEndPosition.x += 50;
}


void SystemAnimationStart::AnimateTotem(){
    int64_t now = Utils::getMillisSinceEpoch();
    int64_t interval = now - timeStartAnim;
    int64_t currentDuration = durations[currentIndexAnimation];
    if (interval < currentDuration) {
        MoveCam(interval, currentDuration);
    } else {
        auto cTransformableCar = static_cast<CTransformable*>(car->GetComponent(CompType::TransformableComp).get());
        cam->setTarget(cTransformableCar->position);
        CalculatePosStartEndRandomCar();
        ResetTimer();
        currentIndexAnimation++;
    }
};

void SystemAnimationStart::AnimateRandomCar(){
    int64_t now = Utils::getMillisSinceEpoch();
    int64_t interval = now - timeStartAnim;
    int64_t currentDuration = durations[currentIndexAnimation];
    if (interval < currentDuration) {
        MoveCam(interval, currentDuration);
    } else {
        CalculatePosStartEndRandomCar();
        ResetTimer();
        currentIndexAnimation++;
    }
};

void SystemAnimationStart::AnimateMainCar(){

};

void SystemAnimationStart::ResetTimer() {
    timeStartAnim = Utils::getMillisSinceEpoch();
}