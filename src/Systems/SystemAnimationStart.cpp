#include "SystemAnimationStart.h"

#include "../Entities/Camera.h"
#include "../Entities/Totem.h"
#include "../Entities/Car.h"
#include "../Systems/Utils.h"

SystemAnimationStart::SystemAnimationStart(Camera *cam_, Totem *totem_, Car *car_) : cam{cam_}, totem{totem_}, car{car_}, timeStartAnim{Utils::getMillisSinceEpoch()} {

}

void SystemAnimationStart::Animate() {
    int64_t now = Utils::getMillisSinceEpoch();
    int64_t interval = now - timeStartAnim;
    cout << "interval -> " << interval << endl;
    float percentTick = std::min(1.0f, (static_cast<float>(interval) / 3000));
    cout << "percentTick -> " << percentTick << endl;
    vec3 currentPos = mix(currentStartPosition, currentEndPosition, percentTick);
    auto cTransfor = static_cast<CTransformable*>(cam->GetComponent(CompType::TransformableComp).get());
    cTransfor->position = currentPos;
    cout << "Current cam pos -> " << currentPos.x << "," << currentPos.y << "," << currentPos.z << endl;
}

void SystemAnimationStart::ResetTimer() {
    timeStartAnim = Utils::getMillisSinceEpoch();
}