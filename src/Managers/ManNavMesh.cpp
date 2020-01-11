#include "./ManNavMesh.h"
#include <iostream>
#include "../Aliases.h"
#include "../Entities/NavMesh.h"
#include "../Components/CDimensions.h"
#include "../Components/CCurrentNavMesh.h"
#include "../Components/CNavMesh.h"

using namespace std;


ManNavMesh::ManNavMesh() {
    //CreatePowerUp(glm::vec3(30.0, 30.0, 30.0));
}

void ManNavMesh::CreateNavMesh(glm::vec3 pos, glm::vec3 rot, float width, float height, float depth, vector<int> waypoints){
    shared_ptr<NavMesh> p = make_shared<NavMesh>(pos,rot,width,height,depth,waypoints);
    entities.push_back(p);
}

void ManNavMesh::UpdateNavMeshTotem(Manager* manTotem){
    if(manTotem->GetEntities().size() > 0 ){
        auto cTransformableTotem = static_cast<CTransformable*>(manTotem->GetEntities()[0].get()->GetComponent(CompType::TransformableComp).get());     
        for(auto navmesh : GetEntities()){
            auto cDimensions = static_cast<CDimensions*>(navmesh.get()->GetComponent(CompType::DimensionsComp).get());
            auto cTransformableNav = static_cast<CTransformable*>(navmesh.get()->GetComponent(CompType::TransformableComp).get()); 
            if( ( (cTransformableTotem->position.x >= (cTransformableNav->position.x-(cDimensions->width/2))) && 
                (cTransformableTotem->position.x <= (cTransformableNav->position.x+(cDimensions->width/2))) ) &&
                ( (cTransformableTotem->position.z >= (cTransformableNav->position.z-(cDimensions->depth/2))) && 
                (cTransformableTotem->position.z <= (cTransformableNav->position.z+(cDimensions->depth/2))) )  ){
                    auto cCurrentNavMesh = static_cast<CCurrentNavMesh*>(manTotem->GetEntities()[0].get()->GetComponent(CompType::CurrentNavMeshComp).get());
                    auto cNavMesh = static_cast<CNavMesh*>(navmesh.get()->GetComponent(CompType::NavMeshComp).get());
                    cCurrentNavMesh->currentNavMesh = cNavMesh->id;
                    //std::cout << " El totem pertenece al naveMesh: " << cNavMesh->id << std::endl;
                }       
        }
    }
}
