#include "SystemHurt.h"

#include "../Entities/Entity.h"
#include "../Components/CHurt.h"
#include "../Components/CTransformable.h"
#include "../Components/CCar.h"
#include "../../include/glm/glm.hpp"
#include "Utils.h"

void SystemHurt::Update(vector<shared_ptr<Entity>> entities) {
    for (auto e : entities) {
        auto cHurt = static_cast<CHurt*>(e->GetComponent(CompType::HurtComp).get());
        if(cHurt->hurt) {
            auto cTransformable = static_cast<CTransformable*>(e->GetComponent(CompType::TransformableComp).get());
            // si hemos acabado las dos vueltas...
            if(cHurt->currentRotation >= cHurt->FINAL_ROTATION) {
                // ponemos hurt a false y reiniciamos estado
                cHurt->hurt = false;
                cHurt->currentRotation = 0;
                cTransformable->rotation.y = cHurt->originalCarRotation;

                // si estamos dando vueltas..
            } else {
                // le quitamos la velocidad al coche
                auto cCar = static_cast<CCar*>(e->GetComponent(CompType::CarComp).get());
                cCar->speed = 0;

                // interpolamos la velocidad de giro para que empiece rápido girando y acabe girando lento
                float percentTick = std::min(1.0f, cHurt->currentRotation / cHurt->FINAL_ROTATION);
                float currentSpeed = mix(cHurt->speedRotationHigh, cHurt->speedRotationLow, percentTick);

                // y finalmente aplicamos la rotación.
                cHurt->currentRotation += currentSpeed;
                cTransformable->rotation.y = cHurt->originalCarRotation + cHurt->currentRotation;
            }
        }
    }
}