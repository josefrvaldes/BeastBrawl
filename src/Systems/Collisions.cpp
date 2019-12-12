#include "Collisions.h"


Collisions::Collisions(){

}

bool Collisions::Intersects(Entity* entity1,Entity* entity2){

    /*
    return (a.minX <= b.maxX && a.maxX >= b.minX) &&
         (a.minY <= b.maxY && a.maxY >= b.minY) &&
         (a.minZ <= b.maxZ && a.maxZ >= b.minZ);

    */
    auto cTransformableE1 = static_cast<CTransformable*>(entity1->GetComponent(CompType::TransformableComp).get());
    auto cDimensionsE1 = static_cast<CDimensions*>(entity1->GetComponent(CompType::DimensionsComp).get());

    auto cTransformableE2 = static_cast<CTransformable*>(entity2->GetComponent(CompType::TransformableComp).get());
    auto cDimensionsE2 = static_cast<CDimensions*>(entity2->GetComponent(CompType::DimensionsComp).get());

    float e1MinX = cTransformableE1->position.x - cDimensionsE1->width/2;
    float e1MinY = cTransformableE1->position.y - cDimensionsE1->height/2;
    float e1MinZ = cTransformableE1->position.z - cDimensionsE1->depth/2;
    float e1MaxX = cTransformableE1->position.x + cDimensionsE1->width/2;
    float e1MaxY = cTransformableE1->position.y + cDimensionsE1->height/2;
    float e1MaxZ = cTransformableE1->position.z + cDimensionsE1->depth/2;

    float e2MinX = cTransformableE2->position.x - cDimensionsE2->width/2;
    float e2MinY = cTransformableE2->position.y - cDimensionsE2->height/2;
    float e2MinZ = cTransformableE2->position.z - cDimensionsE2->depth/2;
    float e2MaxX = cTransformableE2->position.x + cDimensionsE2->width/2;
    float e2MaxY = cTransformableE2->position.y + cDimensionsE2->height/2;
    float e2MaxZ = cTransformableE2->position.z + cDimensionsE2->depth/2;

    if(e1MinX <= e2MaxX && e1MaxX >= e2MinX &&
       e1MinY <= e2MaxY && e1MaxY >= e2MinY &&
       e1MinZ <= e2MaxZ && e1MaxZ >= e2MinZ){
           cout << "chocan\n";
           return true;
    }

    return false;
}