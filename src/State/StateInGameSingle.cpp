#include "StateInGameSingle.h"

#include "../Components/CTotem.h"

StateInGameSingle::StateInGameSingle() : StateInGame() {
    InitVirtualMethods();
}

StateInGameSingle::~StateInGameSingle() {
}

void StateInGameSingle::InitState() {
    StateInGame::InitState();
    CAMBIARCosasNavMesh();
}



void StateInGameSingle::CAMBIARCosasNavMesh(){
    // vamos a asignar el navmesh al que pertenecemos
    for(shared_ptr<Entity> carAI : manCars->GetEntitiesAI()){
        auto cTransformableCar = static_cast<CTransformable*>(carAI.get()->GetComponent(CompType::TransformableComp).get());     
        for(auto navmesh : manNavMesh->GetEntities()){
            auto cDimensions = static_cast<CDimensions*>(navmesh.get()->GetComponent(CompType::DimensionsComp).get());
            auto cTransformableNav = static_cast<CTransformable*>(navmesh.get()->GetComponent(CompType::TransformableComp).get()); 
            if( ( (cTransformableCar->position.x >= (cTransformableNav->position.x-(cDimensions->width/2))) && 
                (cTransformableCar->position.x <= (cTransformableNav->position.x+(cDimensions->width/2))) ) &&
               ( (cTransformableCar->position.z >= (cTransformableNav->position.z-(cDimensions->depth/2))) && 
                (cTransformableCar->position.z <= (cTransformableNav->position.z+(cDimensions->depth/2))) )  ){
                    auto cCurrentNavMesh = static_cast<CCurrentNavMesh*>(carAI.get()->GetComponent(CompType::CurrentNavMeshComp).get());
                    auto cNavMesh = static_cast<CNavMesh*>(navmesh.get()->GetComponent(CompType::NavMeshComp).get());
                    cCurrentNavMesh->currentNavMesh = cNavMesh->id;
                    //std::cout << " El cochecito lereee pertenece al naveMesh: " << cNavMesh->id << std::endl;
                }       
        }
    }   
}

void StateInGameSingle::Input() {
    StateInGame::Input();
}

void StateInGameSingle::Update() {
    StateInGame::Update();

    for (auto actualAI : manCars->GetEntitiesAI()) {
        manCars->UpdateCarAI(actualAI.get(), manPowerUps.get(), manBoxPowerUps.get(), manTotems.get(), manWayPoint.get(), manNavMesh.get(), manBoundingWall.get());
        physicsEngine->UpdateCarAI(actualAI.get());
    }
    CAMBIARCosasDeTotemUpdate();

    // COLISIONES entre powerUp y IA
    collisions->IntersectsCarsPowerUps(manCars.get(), manPowerUps.get(), manNavMesh.get());
    // COLISIONES entre BoxPowerUp y IA
    collisions->IntersectCarsBoxPowerUp(manCars.get(), manBoxPowerUps.get());
    // COLISIONES  entre la IA y el Totem
    collisions->IntersectCarsTotem(manCars.get(), manTotems.get());
}

void StateInGameSingle::Render() {
    auto carAI = manCars->GetEntitiesAI()[0].get();
    bool isColliding = collisions->Intersects(manCars.get()->GetCar().get(), carAI);
    renderEngine->FacadeDrawBoundingBox(manCars.get()->GetCar().get(), isColliding);

    for (auto actualAI : manCars->GetEntitiesAI()) {
        renderEngine->FacadeDrawBoundingBox(actualAI.get(), false);
    }
    renderEngine->FacadeDrawBoundingBox(carAI, isColliding);
    StateInGame::Render();
}

void StateInGameSingle::InitializeCLPhysics(ManCar &manCars, ManBoundingWall &manBoundingWall) {
    StateInGame::InitializeCLPhysics(manCars, manBoundingWall);
}

void StateInGameSingle::InitializeManagers(Physics *physics, Camera *cam) {
    StateInGame::InitializeManagers(physics, cam);
    CAMBIARInicializarCarAIS(*manCars.get(), *manWayPoint.get());
}

void StateInGameSingle::InitializeSystems(ManCar &manCars, ManBoundingWall &manBoundingWall) {
    StateInGame::InitializeSystems(manCars, manBoundingWall);
}

void StateInGameSingle::InitializeFacades() {
    StateInGame::InitializeFacades();
}

void StateInGameSingle::AddElementsToRender() {
    StateInGame::AddElementsToRender();
    for (shared_ptr<Entity> carAI : manCars->GetEntitiesAI())  // Anyadimos los coche IA
        renderEngine->FacadeAddObject(carAI.get());
}

void StateInGameSingle::CAMBIARCosasDeTotemUpdate() {
    bool todosFalse = true;
    auto cTransformTotem = static_cast<CTransformable *>(totemOnCar.get()->GetComponent(CompType::TransformableComp).get());
    cTransformTotem->rotation.y += 0.1;
    for (shared_ptr<Entity> carAI : manCars->GetEntitiesAI()) {  // actualizamos los coche IA
        // comprobamos el componente totem y si lo tienen se lo ponemos justo encima para que se sepa quien lo lleva
        auto cTotem = static_cast<CTotem *>(carAI.get()->GetComponent(CompType::TotemComp).get());
        if (cTotem->active) {
            todosFalse = false;
            auto cTransformCar = static_cast<CTransformable *>(carAI.get()->GetComponent(CompType::TransformableComp).get());
            cTransformTotem->position.x = cTransformCar->position.x;
            cTransformTotem->position.z = cTransformCar->position.z;
            cTransformTotem->position.y = 32.0f;
            // supuestamente esta el drawAll que te lo hace no?????????????????
            // si esta cambiando pero no se esta redibujando
            break; // cuando encontramos a alguien que ya lleva el totem, nos salimos del for, no seguimos comprobando a los demás
        }
    }
    if (todosFalse) {
        auto cTotem = static_cast<CTotem *>(manCars.get()->GetCar().get()->GetComponent(CompType::TotemComp).get());
        if (cTotem->active) {
            auto cTransformCar = static_cast<CTransformable *>(manCars.get()->GetCar().get()->GetComponent(CompType::TransformableComp).get());
            cTransformTotem->position.x = cTransformCar->position.x;
            cTransformTotem->position.z = cTransformCar->position.z;
            cTransformTotem->position.y = 32.0f;
        } else {
            cTransformTotem->position.y = -100.0f;
        }
    }

    renderEngine->UpdateTransformable(totemOnCar.get());
}

void StateInGameSingle::CAMBIARInicializarCarAIS(ManCar &manCars, ManWayPoint &manWayPoint) {
    auto cWayPoint = static_cast<CWayPoint *>(manWayPoint.GetEntities()[2]->GetComponent(CompType::WayPointComp).get());
    //Le asignamos el waypoint inicial, momentaneo a la IA
    manCars.CreateCarAI(glm::vec3(-200.0f, 20.0f, 700.0f), cWayPoint);
    stack<int> pathInit;
    pathInit.push(3);
    pathInit.push(1);
    pathInit.push(2);
    manCars.GetEntitiesAI()[0]->SetPath(pathInit);

    auto cWayPointAI2 = static_cast<CWayPoint *>(manWayPoint.GetEntities()[1]->GetComponent(CompType::WayPointComp).get());
    //Le asignamos el waypoint inicial, momentaneo a la IA
    manCars.CreateCarAI(glm::vec3(400.0f, 20.0f, 20.0f), cWayPointAI2);
    stack<int> pathInit2;
    pathInit2.push(4);
    pathInit2.push(0);
    pathInit2.push(2);
    manCars.GetEntitiesAI()[1]->SetPath(pathInit2);

    auto cWayPointAI3 = static_cast<CWayPoint *>(manWayPoint.GetEntities()[0]->GetComponent(CompType::WayPointComp).get());
    //Le asignamos el waypoint inicial, momentaneo a la IA
    manCars.CreateCarAI(glm::vec3(400.0f, 20.0f, -400.0f), cWayPointAI3);
    stack<int> pathInit3;
    pathInit3.push(5);
    pathInit3.push(0);
    pathInit3.push(5);
    manCars.GetEntitiesAI()[2]->SetPath(pathInit3);

}