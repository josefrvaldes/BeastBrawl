#include "StateInGameSingle.h"

StateInGameSingle::StateInGameSingle() : StateInGame() {
    systemBtPowerUp = make_unique<SystemBtPowerUp>();
    systemBtMoveTo = make_unique<SystemBtMoveTo>();
    systemBtLoDMove = make_unique<SystemBtLoDMove>();
    systemPathPlanning = make_unique<SystemPathPlanning>();

    InitState();
    InitVirtualMethods();

    //CAMBIARCosasNavMesh(*manCars.get(), *manNavMesh.get());

}

StateInGameSingle::~StateInGameSingle() {
}

void StateInGameSingle::InitState() {
    StateInGame::InitState();
}


/*
void StateInGameSingle::CAMBIARCosasNavMesh(ManCar &manCars, ManNavMesh &manNavMesh){
    // vamos a asignar el navmesh al que pertenecemos
    for(auto& carAI : manCars.GetEntities()){
        if(static_cast<Car*>(carAI.get())->GetTypeCar() == TypeCar::CarAI){
            auto cTransformableCar = static_cast<CTransformable*>(carAI.get()->GetComponent(CompType::TransformableComp).get());     
            for(auto& navmesh : manNavMesh.GetEntities()){
                auto cDimensions = static_cast<CDimensions*>(navmesh.get()->GetComponent(CompType::DimensionsComp).get());
                auto cTransformableNav = static_cast<CTransformable*>(navmesh.get()->GetComponent(CompType::TransformableComp).get()); 
                if( ( (cTransformableCar->position.x >= (cTransformableNav->position.x-(cDimensions->width/2))) && 
                    (cTransformableCar->position.x <= (cTransformableNav->position.x+(cDimensions->width/2))) ) &&
                ( (cTransformableCar->position.z >= (cTransformableNav->position.z-(cDimensions->depth/2))) && 
                    (cTransformableCar->position.z <= (cTransformableNav->position.z+(cDimensions->depth/2))) )  ){
                        auto cCurrentNavMesh = static_cast<CCurrentNavMesh*>(carAI.get()->GetComponent(CompType::CurrentNavMeshComp).get());
                        auto cNavMesh = static_cast<CNavMesh*>(navmesh.get()->GetComponent(CompType::NavMeshComp).get());
                        cCurrentNavMesh->currentNavMesh = cNavMesh->id;
                        //std::cout << " EL NAVMESH DE LAS IA ES::::234563345677: " << cNavMesh->id << std::endl;
                    }       
            }
        }
    }   
}
*/


void StateInGameSingle::Input() {
    renderEngine->FacadeCheckInputSingle();
    inputEngine->CheckInput();
}

void StateInGameSingle::Update() {
    StateInGame::Update();
    for (auto actualAI : manCars->GetEntities()) { // CUIDADO!!! -> el static cast que solo se use en el single player, si no peta
        if (static_cast<Car*>(actualAI.get())->GetTypeCar() == TypeCar::CarAI){
            manCars->UpdateCarAI(
                static_cast<CarAI*>(actualAI.get()), 
                manPowerUps.get(), 
                manBoxPowerUps.get(), 
                manTotems.get(), 
                manWayPoint.get(), 
                manNavMesh.get(), 
                manBoundingWall.get(), 
                systemBtPowerUp.get(), 
                systemBtMoveTo.get(), 
                systemBtLoDMove.get(),
                systemPathPlanning.get());
        }
    }
    //CAMBIARCosasDeTotemUpdate();

    // COLISIONES entre powerUp y IA
    //collisions->IntersectsCarsPowerUps(manCars.get(), manPowerUps.get(), manNavMesh.get());
    // COLISIONES entre BoxPowerUp y IA
    //collisions->IntersectCarsBoxPowerUp(manCars.get(), manBoxPowerUps.get());
    // COLISIONES  entre la IA y el Totem
    //collisions->IntersectCarsTotem(manCars.get(), manTotems.get());

    // Actualizamos posicion en Irrlicht
    for (auto actualAI : manCars->GetEntities()) { // CUIDADO!!! -> el static cast que solo se use en el single player, si no peta
        if (static_cast<Car*>(actualAI.get())->GetTypeCar() == TypeCar::CarAI){
            physicsEngine->UpdateTransformable(actualAI.get());
        }
    }


}

void StateInGameSingle::Render() {
    //auto carPrincial = manCars->GetCar().get();
    //bool isColliding = collisions->Intersects(manCars.get()->GetCar().get(), carPrincial);
    //renderEngine->FacadeDrawBoundingBox(manCars.get()->GetCar().get(), isColliding);

    for (auto cars : manCars->GetEntities()) {
        renderEngine->FacadeDrawBoundingBox(cars.get(), false);
    }
    //renderEngine->FacadeDrawBoundingBox(carPrincial, isColliding);
    StateInGame::Render();
}

void StateInGameSingle::InitializeCLPhysics(ManCar &manCars, ManBoundingWall &manWall, ManBoundingOBB &manOBB, ManBoundingGround &manGround, ManPowerUp &manPowerUp, ManNavMesh &manNavMesh, ManBoxPowerUp &manBoxPowerUp, ManTotem &manTotem) {
    StateInGame::InitializeCLPhysics(manCars, manWall, manOBB, manGround, manPowerUp, manNavMesh, manBoxPowerUp, manTotem);
}

void StateInGameSingle::InitializeManagers(Physics *physics, Camera *cam) {
    StateInGame::InitializeManagers(physics, cam);
    CAMBIARInicializarCarAIS(*manCars, *manWayPoint);
}

void StateInGameSingle::InitializeSystems(ManCar &manCars, ManBoundingWall &manWall, ManBoundingOBB &manOBB, ManBoundingGround &manGround, ManPowerUp &manPowerUp, ManNavMesh &manNavMesh, ManBoxPowerUp &manBoxPowerUp, ManTotem &manTotem) {
    StateInGame::InitializeSystems(manCars, manWall, manOBB, manGround, manPowerUp, manNavMesh, manBoxPowerUp, manTotem);
}

void StateInGameSingle::InitializeFacades() {
    StateInGame::InitializeFacades();
}

void StateInGameSingle::AddElementsToRender() {
    StateInGame::AddElementsToRender();
    
}

/*
void StateInGameSingle::CAMBIARCosasDeTotemUpdate() {
    bool todosFalse = true;
    auto cTransformTotem = static_cast<CTransformable *>(totemOnCar->GetComponent(CompType::TransformableComp).get());
    cTransformTotem->rotation.y += 0.1;
    for (const auto& carAI : manCars->GetEntities()) {  // actualizamos los coche IA
        // comprobamos el componente totem y si lo tienen se lo ponemos justo encima para que se sepa quien lo lleva
        auto cTotem = static_cast<CTotem *>(carAI.get()->GetComponent(CompType::TotemComp).get());
        if (cTotem->active) {
            todosFalse = false;
            auto cTransformCar = static_cast<CTransformable *>(carAI.get()->GetComponent(CompType::TransformableComp).get());
            cTransformTotem->position.x = cTransformCar->position.x;
            cTransformTotem->position.z = cTransformCar->position.z;
            cTransformTotem->position.y = 22.0f;
            // supuestamente esta el drawAll que te lo hace no?????????????????
            // si esta cambiando pero no se esta redibujando
            break; // cuando encontramos a alguien que ya lleva el totem, nos salimos del for, no seguimos comprobando a los demás
        }
    }
    if(todosFalse){
        cTransformTotem->position.y = -100.0f;
    }

    renderEngine->UpdateTransformable(totemOnCar.get());
}
*/

void StateInGameSingle::CAMBIARInicializarCarAIS(ManCar &manCars, ManWayPoint &manWayPoint) {
/*    
    auto cWayPoint = static_cast<CWayPoint *>(manWayPoint.GetEntities()[0]->GetComponent(CompType::WayPointComp).get());
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
*/
    auto posCar1 = glm::vec3(0.0f, 15.0f, -200.0f);
    auto posCar2 = glm::vec3(-202.0f, 15.0f, -145.0f);
    auto posCar3 = glm::vec3(209.0f, 15.0f, -145.0f);

    manCars.CreateCarAI(posCar1);
    manCars.CreateCarAI(posCar2);
    manCars.CreateCarAI(posCar3);

    //int i = -1;
    //TODO: Cambiar de sitio
    for (auto e : manCars.GetEntities()) {
        //i++;
        //if (i != 0) {
            auto idComp = static_cast<CId*>(e->GetComponent(CompType::IdComp).get());
            auto posComp = static_cast<CTransformable*>(e->GetComponent(CompType::TransformableComp).get());
            string nameEvent = "Coche/motor";
            SoundFacadeManager::GetInstance()->GetSoundFacade()->CreateSoundDinamic3D(idComp->id, posComp->position, nameEvent, 1, 0);
            nameEvent = "PowerUp/escudo";
            SoundFacadeManager::GetInstance()->GetSoundFacade()->CreateSoundDinamic3D(idComp->id, posComp->position, nameEvent, 0, 0);
            nameEvent = "PowerUp/escudo_roto";
            SoundFacadeManager::GetInstance()->GetSoundFacade()->CreateSoundEstatic3D(idComp->id, posComp->position, nameEvent, 0);
            nameEvent = "Coche/choque_powerup";
            SoundFacadeManager::GetInstance()->GetSoundFacade()->CreateSoundEstatic3D(idComp->id, posComp->position, nameEvent, 0);
            nameEvent = "Coche/choque";
            SoundFacadeManager::GetInstance()->GetSoundFacade()->CreateSoundEstatic3D(idComp->id, posComp->position, nameEvent, 0);
        //}
    }
}