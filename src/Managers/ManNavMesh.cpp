#include "./ManNavMesh.h"
#include <iostream>
#include "../Aliases.h"
#include "../Entities/NavMesh.h"
#include "../Entities/Totem.h"
#include "../Components/CDimensions.h"
#include "../Components/CCurrentNavMesh.h"
#include "../Components/CNavMesh.h"

using namespace std;


ManNavMesh::ManNavMesh() {
    // CREAMOS DE PRUEBA UN NAVMESH
    vector<int> waypoints0{0,1,2,3,4,12};
    vector<int> waypoints1{7,8,9,10,11,13};
    vector<int> waypoints2{3,5,7};  // el 5 debe ser referencia
    vector<int> waypoints3{4,6,8};  // el 6 debe ser referencia
    CreateNavMesh(glm::vec3(0.0f,0.0f,-200.0f),glm::vec3(0.0f,0.0f,0.0f),1000,32,500,waypoints0);  //0
    CreateNavMesh(glm::vec3(0.0f,0.0f,500.0f),glm::vec3(0.0f,0.0f,0.0f),1000,32,500,waypoints1);   //1
    CreateNavMesh(glm::vec3(-300.0f,0.0f,150.0f),glm::vec3(0.0f,0.0f,0.0f),150,32,200,waypoints2); //2
    CreateNavMesh(glm::vec3(300.0f,0.0f,150.0f),glm::vec3(0.0f,0.0f,0.0f),150,32,200,waypoints3);  //3

    //nos suscribimos
    SubscribeToEvents();
}

void ManNavMesh::CreateNavMesh(glm::vec3 pos, glm::vec3 rot, float width, float height, float depth, vector<int> waypoints){
    shared_ptr<NavMesh> p = make_shared<NavMesh>(pos,rot,width,height,depth,waypoints);
    entities.push_back(p);
}


void ManNavMesh::SubscribeToEvents() {

    EventManager::GetInstance().SuscribeMulti(Listener(
        EventType::ACTUALIZE_NAVMESH_TOTEM,
        bind(&ManNavMesh::ActualizeNavMeshTotem, this, placeholders::_1),
        "ActualizeNavMeshTotem"));
    EventManager::GetInstance().SuscribeMulti(Listener(
        EventType::ACTUALIZE_NAVMESH_CARAI,
        bind(&ManNavMesh::ActualizeNavMeshCarAI, this, placeholders::_1),
        "ActualizeNavMeshCarAI"));
}

void ManNavMesh::ActualizeNavMeshTotem(DataMap* d){
    //std::cout << "Actualizamos NavMesh del Totem" << std::endl;
    bool todoCorrecto = false;
    auto cTransformableTotem = static_cast<CTransformable*>(any_cast<Entity*>((*d)["totem"])->GetComponent(CompType::TransformableComp).get());     
    for(auto navmesh : GetEntities()){
        auto cDimensions = static_cast<CDimensions*>(navmesh.get()->GetComponent(CompType::DimensionsComp).get());
        auto cTransformableNav = static_cast<CTransformable*>(navmesh.get()->GetComponent(CompType::TransformableComp).get()); 
        if( ( (cTransformableTotem->position.x >= (cTransformableNav->position.x-(cDimensions->width/2))) && 
            (cTransformableTotem->position.x <= (cTransformableNav->position.x+(cDimensions->width/2))) ) &&
            ( (cTransformableTotem->position.z >= (cTransformableNav->position.z-(cDimensions->depth/2))) && 
            (cTransformableTotem->position.z <= (cTransformableNav->position.z+(cDimensions->depth/2))) )  ){
                auto cCurrentNavMesh = static_cast<CCurrentNavMesh*>(any_cast<Entity*>((*d)["totem"])->GetComponent(CompType::CurrentNavMeshComp).get());
                auto cNavMesh = static_cast<CNavMesh*>(navmesh.get()->GetComponent(CompType::NavMeshComp).get());
                cCurrentNavMesh->currentNavMesh = cNavMesh->id;
                //std::cout << " El totem pertenece al naveMesh: " << cNavMesh->id << std::endl;
                todoCorrecto = true;
            }       
    }
    if(!todoCorrecto){
        // hay que comprobar tambien si donde lo queremos crear esta dentro de un objeto (colisionando con un objeto)
        std::cout << "ALGOOOOOOOOOOOOO VAAAAAAAAAAAAAAAAAAAAAAAA MAAAAAAAAAAAAAAAAAAAAAAAAAAAAL" << std::endl;
        std::cout << "El totem no esta en ningun navMesh, y como por defecto se pone el -1... PETA" << std::endl;

    }
}
void ManNavMesh::ActualizeNavMeshCarAI(DataMap* d){
    auto cTransformableCar = static_cast<CTransformable*>(any_cast<Entity*>((*d)["carAI"])->GetComponent(CompType::TransformableComp).get());     
    for(auto navmesh : GetEntities()){
        auto cDimensions = static_cast<CDimensions*>(navmesh.get()->GetComponent(CompType::DimensionsComp).get());
        auto cTransformableNav = static_cast<CTransformable*>(navmesh.get()->GetComponent(CompType::TransformableComp).get()); 
        if( ( (cTransformableCar->position.x >= (cTransformableNav->position.x-(cDimensions->width/2))) && 
            (cTransformableCar->position.x <= (cTransformableNav->position.x+(cDimensions->width/2))) ) &&
            ( (cTransformableCar->position.z >= (cTransformableNav->position.z-(cDimensions->depth/2))) && 
            (cTransformableCar->position.z <= (cTransformableNav->position.z+(cDimensions->depth/2))) )  ){
                auto cCurrentNavMesh = static_cast<CCurrentNavMesh*>(any_cast<Entity*>((*d)["carAI"])->GetComponent(CompType::CurrentNavMeshComp).get());
                auto cNavMesh = static_cast<CNavMesh*>(navmesh.get()->GetComponent(CompType::NavMeshComp).get());
                cCurrentNavMesh->currentNavMesh = cNavMesh->id;
                //std::cout << " El cochecito lereee pertenece al naveMesh: " << cNavMesh->id << std::endl;
            }       
    }
}

void ManNavMesh::UpdateNavMeshPlayer(Entity* carPlayer){
    auto cTransformableCar = static_cast<CTransformable*>(carPlayer->GetComponent(CompType::TransformableComp).get());     
    for(auto navmesh : GetEntities()){
        auto cDimensions = static_cast<CDimensions*>(navmesh.get()->GetComponent(CompType::DimensionsComp).get());
        auto cTransformableNav = static_cast<CTransformable*>(navmesh.get()->GetComponent(CompType::TransformableComp).get()); 
        if( ( (cTransformableCar->position.x >= (cTransformableNav->position.x-(cDimensions->width/2))) && 
            (cTransformableCar->position.x <= (cTransformableNav->position.x+(cDimensions->width/2))) ) &&
            ( (cTransformableCar->position.z >= (cTransformableNav->position.z-(cDimensions->depth/2))) && 
            (cTransformableCar->position.z <= (cTransformableNav->position.z+(cDimensions->depth/2))) )  ){
                auto cCurrentNavMesh = static_cast<CCurrentNavMesh*>(carPlayer->GetComponent(CompType::CurrentNavMeshComp).get());
                auto cNavMesh = static_cast<CNavMesh*>(navmesh.get()->GetComponent(CompType::NavMeshComp).get());
                cCurrentNavMesh->currentNavMesh = cNavMesh->id;
                //std::cout << " El cochecito lereee pertenece al naveMesh: " << cNavMesh->id << std::endl;
            }       
    }
}
