#include "SystemHurt.h"

#include "../Entities/Entity.h"
#include "../Components/CHurt.h"
#include "../Components/CTransformable.h"
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
                cHurt->currentRotation += cHurt->speedRotation;
                // cout << "El ctransformable.rotation era " << cTransformable->rotation.y << " y el hurt rotation es " << cHurt->currentRotation << endl;
                cTransformable->rotation.y += cHurt->speedRotation;
                // cout << "Tras sumar, el ctransformable.rotation es " << cTransformable->rotation.y << endl;
            }
        }
    }
}