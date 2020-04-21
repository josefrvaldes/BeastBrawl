#include "StateInGameSingle.h"

#include "../Managers/ManAI.h"

StateInGameSingle::StateInGameSingle() : StateInGame() {

    InitState();
    InitVirtualMethods();

    createSystemAI();

    //CAMBIARCosasNavMesh(*manCars.get(), *manNavMesh.get());

}

StateInGameSingle::~StateInGameSingle() {
}

void StateInGameSingle::InitState() {
    StateInGame::InitState();
}


void StateInGameSingle::Input() {
    renderEngine->FacadeCheckInputSingle();
    inputEngine->CheckInputSingle();
}

void StateInGameSingle::Update() {
    StateInGame::Update();


    //std::cout << "LOS TIEMPOS SON:  ";
    //cout << " ------------------------------------------------------------------------------- " << endl;
    manAI->Update();


    for (auto actualAI : manCars->GetEntities()) { // CUIDADO!!! -> el static cast que solo se use en el single player, si no peta
        if (static_cast<Car*>(actualAI.get())->GetTypeCar() == TypeCar::CarAI){
            manCars->UpdateCarAI(static_cast<CarAI*>(actualAI.get()),manTotems.get());
        }
    }

    //std::cout << " " << std::endl;
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

void StateInGameSingle::InitializeManagers(Physics *physics, Camera *cam, const uint32_t timeGame) {
    StateInGame::InitializeManagers(physics, cam, timeGame);

    //Cambiar
    StateInGame::CreateMainCar();
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


void StateInGameSingle::createSystemAI(){
    // creamos ManAI
    manAI = make_unique<ManAI>();


    // iniciamos los sistemas
    InitBtMoveTo();
    InitPathPlanning();
    InitBtLoDMove();
    InitBtPowerUp();

    //creamos comportamientos IA
    uint32_t i = 0;
    for(auto actualAI : manCars->GetEntities()){
        if (static_cast<Car*>(actualAI.get())->GetTypeCar() == TypeCar::CarAI){
            manAI->addBehavior(static_cast<CarAI*>(actualAI.get()), systemBtMoveTo.get(),     systemBtMoveTo->getFrecuency(),     i, systemBtMoveTo.get()->getMaxProcessTime() );
            manAI->addBehavior(static_cast<CarAI*>(actualAI.get()), systemPathPlanning.get(), systemPathPlanning->getFrecuency(), i, systemPathPlanning.get()->getMaxProcessTime() );
            manAI->addBehavior(static_cast<CarAI*>(actualAI.get()), systemBtLoDMove.get(),    systemBtLoDMove->getFrecuency(),    i, systemBtLoDMove.get()->getMaxProcessTime() );
            manAI->addBehavior(static_cast<CarAI*>(actualAI.get()), systemBtPowerUp.get(),    systemBtPowerUp->getFrecuency(),    i, systemBtPowerUp.get()->getMaxProcessTime() );
            
            i++;
        }
    }
}


void StateInGameSingle::InitBtPowerUp(){
    systemBtPowerUp = make_unique<SystemBtPowerUp>();

    systemBtPowerUp->AddManager(*manCars.get());
    systemBtPowerUp->AddManager(*manPowerUps.get());
    systemBtPowerUp->AddManager(*manBoxPowerUps.get());
    systemBtPowerUp->AddManager(*manTotems.get());
    systemBtPowerUp->AddManager(*manWayPoint.get());
    systemBtPowerUp->AddManager(*manNavMesh.get());
    systemBtPowerUp->AddManager(*manBoundingWall.get());
    systemBtPowerUp->AddManager(*manBoundingOBB.get());
    // Precalculado
    systemBtPowerUp->setMaxProcessTime(0.00025);
}
void StateInGameSingle::InitBtMoveTo(){
    systemBtMoveTo = make_unique<SystemBtMoveTo>();

    systemBtMoveTo->AddManager(*manCars.get());
    systemBtMoveTo->AddManager(*manPowerUps.get());
    systemBtMoveTo->AddManager(*manBoxPowerUps.get());
    systemBtMoveTo->AddManager(*manTotems.get());
    systemBtMoveTo->AddManager(*manWayPoint.get());
    systemBtMoveTo->AddManager(*manNavMesh.get());
    systemBtMoveTo->AddManager(*manBoundingWall.get());
    systemBtMoveTo->AddManager(*manBoundingOBB.get());

    systemBtMoveTo->setMaxProcessTime(0.00035);
}
void StateInGameSingle::InitBtLoDMove(){
    systemBtLoDMove = make_unique<SystemBtLoDMove>();

    systemBtLoDMove->AddManager(*manCars.get());
    systemBtLoDMove->AddManager(*manPowerUps.get());
    systemBtLoDMove->AddManager(*manBoxPowerUps.get());
    systemBtLoDMove->AddManager(*manTotems.get());
    systemBtLoDMove->AddManager(*manWayPoint.get());
    systemBtLoDMove->AddManager(*manNavMesh.get());
    systemBtLoDMove->AddManager(*manBoundingWall.get());
    systemBtLoDMove->AddManager(*manBoundingOBB.get());

    systemBtLoDMove->setMaxProcessTime(0.00053);
}
void StateInGameSingle::InitPathPlanning(){
    systemPathPlanning = make_unique<SystemPathPlanning>();

    systemPathPlanning->AddManager(*manCars.get());
    systemPathPlanning->AddManager(*manPowerUps.get());
    systemPathPlanning->AddManager(*manBoxPowerUps.get());
    systemPathPlanning->AddManager(*manTotems.get());
    systemPathPlanning->AddManager(*manWayPoint.get());
    systemPathPlanning->AddManager(*manNavMesh.get());
    systemPathPlanning->AddManager(*manBoundingWall.get());
    systemPathPlanning->AddManager(*manBoundingOBB.get());

    systemPathPlanning->setMaxProcessTime(0.00025);
}





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

    //Cambiar
    manCars.CreateCarAI(0, posCar1);
    manCars.CreateCarAI(0, posCar2);
    manCars.CreateCarAI(0, posCar3);

    //int i = -1;
    //TODO: Cambiar de sitio
    for (auto e : manCars.GetEntities()) {
        //i++;
        //if (i != 0) {
            auto idComp = static_cast<CId*>(e->GetComponent(CompType::IdComp).get());
            auto posComp = static_cast<CTransformable*>(e->GetComponent(CompType::TransformableComp).get());
            string nameEvent = "Coche/motor";
            SoundFacadeManager::GetInstance()->GetSoundFacade()->CreateSoundDinamic3D(idComp->id, posComp->position, nameEvent, 1, 0);
            nameEvent = "Coche/motor" + idComp->id;
            SoundFacadeManager::GetInstance()->GetSoundFacade()->SetParameter(nameEvent, "personaje", 6);
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