#include "StateInGameSingle.h"

StateInGameSingle::StateInGameSingle() : StateInGame() {
    systemBtPowerUp = make_unique<SystemBtPowerUp>();
    systemBtMoveTo = make_unique<SystemBtMoveTo>();
    systemBtLoDMove = make_unique<SystemBtLoDMove>();
    systemPathPlanning = make_unique<SystemPathPlanning>();

    InitVirtualMethods();

    //std::cout << "ENTRAMOS AL MANAGER DE NAVMESH LOCOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO" << std::endl;
    CAMBIARCosasNavMesh(*manCars.get(), *manNavMesh.get());
    //std::cout << "despues de la llamada LOOOOOOOOOOOCOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO" << std::endl;
}

StateInGameSingle::~StateInGameSingle() {
}

void StateInGameSingle::InitState() {
    StateInGame::InitState();
}





void StateInGameSingle::Input() {
    renderEngine->FacadeCheckInputSingle();
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
    CAMBIARPositionTotemAboveCar();

    // COLISIONES entre powerUp y IA
    collisions->IntersectsCarsPowerUps(manCars.get(), manPowerUps.get(), manNavMesh.get());
    // COLISIONES entre BoxPowerUp y IA
    collisions->IntersectCarsBoxPowerUp(manCars.get(), manBoxPowerUps.get());
    // COLISIONES  entre la IA y el Totem
    collisions->IntersectCarsTotem(manCars.get(), manTotems.get());

    // Actualizamos posicion en Irrlicht
    for (auto actualAI : manCars->GetEntities()) { // CUIDADO!!! -> el static cast que solo se use en el single player, si no peta
        if (static_cast<Car*>(actualAI.get())->GetTypeCar() == TypeCar::CarAI){
            physicsEngine->UpdateCarAI(actualAI.get());
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

void StateInGameSingle::InitializeCLPhysics(ManCar &manCars, ManBoundingWall &manBoundingWall, ManBoundingOBB &manBoundingOBB) {
    StateInGame::InitializeCLPhysics(manCars, manBoundingWall, manBoundingOBB);
}

void StateInGameSingle::InitializeManagers(Physics *physics, Camera *cam) {
    StateInGame::InitializeManagers(physics, cam);
    CAMBIARInicializarCarAIS(*manCars, *manWayPoint);
}

void StateInGameSingle::InitializeSystems(ManCar &manCars, ManBoundingWall &manBoundingWall, ManBoundingOBB &manBoundingOBB) {
    StateInGame::InitializeSystems(manCars, manBoundingWall, manBoundingOBB);
}

void StateInGameSingle::InitializeFacades() {
    StateInGame::InitializeFacades();
}

void StateInGameSingle::AddElementsToRender() {
    StateInGame::AddElementsToRender();
    
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
    manCars.CreateCarAI(glm::vec3(-200.0f, 10.0f, 700.0f));
    manCars.CreateCarAI(glm::vec3(400.0f, 10.0f, -50.0f));
    //manCars.CreateHumanCar(glm::vec3(20.0, 10.0, 20.0));
    manCars.CreateCarAI(glm::vec3(300.0f, 10.0f, -300.0f));
}