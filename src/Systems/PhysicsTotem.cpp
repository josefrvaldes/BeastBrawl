#include "PhysicsTotem.h"
#include "../Constants.h"
#include "Utils.h"


PhysicsTotem::PhysicsTotem(){

}

void PhysicsTotem::update(Entity* totem){
   // lo dejamos para cuando tengamos que animarlo
      auto cTotem = static_cast<CTotem*>(totem->GetComponent(CompType::TotemComp).get());
        auto cTransformable = static_cast<CTransformable*>(totem->GetComponent(CompType::TransformableComp).get());
      if(cTotem->speed < cTotem->MaxSpeed){
        // Movimiento
        float angleRotation = (cTransformable->rotation.y * M_PI) / 180.0;
        cTransformable->position.x -= cos(angleRotation) * cTotem->speed * Constants::DELTA_TIME;
        cTransformable->position.z += sin(angleRotation) * cTotem->speed * Constants::DELTA_TIME;
        cTotem->speed += cTotem->aceleration;
      }else{
        if(cTotem->active){
          cTransformable->rotation.y += 100 * Constants::DELTA_TIME;
        }
      }
}