#include "SystemHurt.h"

#include "../Entities/Entity.h"
#include "../Components/CHurt.h"
#include "../Components/CTransformable.h"
#include "Utils.h"

void SystemHurt::Update(vector<shared_ptr<Entity>> entities) {
    for (auto e : entities) {
        auto cHurt = static_cast<CHurt*>(e->GetComponent(CompType::HurtComp).get());
        if(cHurt->hurt) {
            int64_t now = Utils::getMillisSinceEpoch();
            int64_t interval = now - cHurt->timeStartHurt;
            if(interval > cHurt->MAX_TIME_HURT) {
                cHurt->hurt = false;
                cHurt->currentRotation = 0;
            } else {
                auto cTransformable = static_cast<CTransformable*>(e->GetComponent(CompType::TransformableComp).get());
                cTransformable->rotation.y += cHurt->currentRotation;
            }
        }
    }
}