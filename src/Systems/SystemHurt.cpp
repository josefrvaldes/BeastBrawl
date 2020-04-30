#include "SystemHurt.h"

#include "../Entities/Entity.h"
#include "../Components/CHurt.h"
#include "../Components/CTransformable.h"
#include "../Components/CCar.h"
#include "Utils.h"

void SystemHurt::Update(vector<shared_ptr<Entity>> entities) {
    for (auto e : entities) {
        auto cHurt = static_cast<CHurt*>(e->GetComponent(CompType::HurtComp).get());
        if(cHurt->hurt) {
            auto cTransformable = static_cast<CTransformable*>(e->GetComponent(CompType::TransformableComp).get());
            if(cHurt->currentRotation >= 720) {
                cHurt->hurt = false;
                cHurt->currentRotation = 0;
                cTransformable->rotation.y = cHurt->originalCarRotation;
            } else {
                auto cCar = static_cast<CCar*>(e->GetComponent(CompType::CarComp).get());
                cCar->speed = 0;
                cHurt->currentRotation += cHurt->speedRotation;
                cTransformable->rotation.y = cHurt->originalCarRotation + cHurt->currentRotation;
            }
        }
    }
}