#include "StateInGameMulti.h"
#include <Components/CBufferOnline.h>
#include <Components/CTotem.h>
#include "../Components/COnline.h"
#include "../Systems/SystemOnline.h"
#include "../Systems/Utils.h"

StateInGameMulti::StateInGameMulti(uint16_t IdOnline, const vector<uint16_t> IdPlayersOnline) : StateInGame() {
    InitVirtualMethods();
    // a este le llegan los coches
    //std::cout << "POR FIIIIIIIIIIIIIIIIIIIIIIIN: " << std::endl;
    vector<uint16_t> arrayIdEnemies = IdPlayersOnline;

    sysOnline = make_unique<SystemOnline>(*manCars, IdOnline);

    vec3 posIniciales[] = {
        vec3(120.0f, 10.0f, -300.0f),
        vec3(20.0f, 10.0f, -300.0f),
        vec3(40.0f, 10.0f, -150.0f),
        vec3(-50.0f, 10.0f, -50.0f),
        vec3(50.0f, 10.0f, -200.0f),
        vec3(0.0f, 10.0f, 0.0f)};

    auto cTransformable = static_cast<CTransformable *>(manCars->GetCar()->GetComponent(CompType::TransformableComp).get());
    cTransformable->position = posIniciales[IdOnline - 1];
    COnline *cOnline = static_cast<COnline *>(manCars->GetCar()->GetComponent(CompType::OnlineComp).get());
    cOnline->idClient = IdOnline;

    for (auto idEnemy : IdPlayersOnline) {
        vec3 pos = posIniciales[idEnemy - 1];
        manCars->CreateHumanCar(pos);
        shared_ptr<Entity> car = manCars->GetEntities()[manCars->GetEntities().size() - 1];
        COnline *cOnline = static_cast<COnline *>(car->GetComponent(CompType::OnlineComp).get());
        cOnline->idClient = idEnemy;
        renderEngine->FacadeAddObject(car.get());
        // cout << "El coche con id online " << idEnemy << " está en la pos " << pos.x << "," << pos.y << "," << pos.z << endl;
    }
    vector<Constants::InputTypes> inputs;
    sysOnline->SendInputs(inputs);  // enviamos un vector vacío la primera vez para que el servidor sepa que estamos vivos

    CAMBIARCosasNavMesh(*manCars.get(), *manNavMesh.get());
    // while(true){sleep(500);}; // esto solo sirve para depurar


    for (const auto &car : manCars->GetEntities()) {
        const auto cTransformable = static_cast<CTransformable *>(manCars->GetCar()->GetComponent(CompType::TransformableComp).get());

        shared_ptr<CBufferOnline> buffer = make_shared<CBufferOnline>();        
        BuffElement elem(inputs, cTransformable->position, cTransformable->rotation);
        buffer->elems.push_back(elem);

        car->AddComponent(buffer);
    }
}

StateInGameMulti::~StateInGameMulti() {
}

void StateInGameMulti::InitState() {
    StateInGame::InitState();
}

void StateInGameMulti::Input() {
    const vector<Constants::InputTypes> &inputs = renderEngine->FacadeCheckInputMulti();
    if (previousInputs != inputs) {
        cout << Utils::getISOCurrentTimestampMillis() << " [" << sysOnline->idOnlineMainCar << "] Enviamos los inputs porque han cambiado con respecto a la iteración anterior" << endl;
        sysOnline->SendInputs(inputs);
        previousInputs = inputs;
    }

    time_point<system_clock> now = system_clock::now();
    auto millisSinceLastInputSent = duration_cast<milliseconds>(now - lastTimeSentInputs).count();
    if (millisSinceLastInputSent > 66) {  // 100 = 10fps; 66 = 15fps   1000 = 60fps
        lastTimeSentInputs = now;
        sysOnline->SendInputs(inputs);
    }

    auto millisSinceLastSyncSent = duration_cast<milliseconds>(now - lastTimeSentSync).count();
    if (millisSinceLastSyncSent > 200) {  // 1000ms = 1s = 60fps; 2s = 120frames
        lastTimeSentSync = now;
        sysOnline->SendSync(manCars.get(), manTotems.get());
    }
}

void StateInGameMulti::Update() {
    StateInGame::Update();

    for (auto actualCar : manCars->GetEntities()) {
        if (actualCar.get() != manCars->GetCar().get()) {
            manNavMesh->UpdateNavMeshHuman(actualCar.get());  // actualizamos el navemesh en el que se encuentra al human
            // funcion para recibir los inputs del servidor, otra para enviar los nuestros, crear componente de input
            physics->UpdateHuman(static_cast<Car *>(actualCar.get()));
            manCars->UpdateCarHuman(actualCar.get());
            physicsEngine->UpdateTransformable(actualCar.get());
        }
    }

    CAMBIARPositionTotemAboveCar();
    // COLISIONES entre powerUp y cocheHuman
    //collisions->IntersectsCarsPowerUps(manCars.get(), manPowerUps.get(), manNavMesh.get());
    // COLISIONES entre BoxPowerUp y cocheHuman
    //collisions->IntersectCarsBoxPowerUp(manCars.get(), manBoxPowerUps.get());
    // COLISIONES  entre la cocheHuman y el Totem
    //collisions->IntersectCarsTotem(manCars.get(), manTotems.get());
}

void StateInGameMulti::Render() {
    renderEngine->FacadeDrawBoundingBox(manCars.get()->GetCar().get(), true);
    StateInGame::Render();
}

void StateInGameMulti::InitializeCLPhysics(ManCar &manCars, ManBoundingWall &manWall, ManBoundingOBB &manOBB, ManBoundingGround &manGround, ManPowerUp &manPowerUp, ManNavMesh &manNavMesh, ManBoxPowerUp &manBoxPowerUp, ManTotem &manTotem) {
    StateInGame::InitializeCLPhysics(manCars, manWall, manOBB, manGround, manPowerUp, manNavMesh, manBoxPowerUp, manTotem);
}

void StateInGameMulti::InitializeManagers(Physics *physics, Camera *cam) {
    StateInGame::InitializeManagers(physics, cam);
}

void StateInGameMulti::InitializeSystems(ManCar &manCars, ManBoundingWall &manWall, ManBoundingOBB &manOBB, ManBoundingGround &manGround, ManPowerUp &manPowerUp, ManNavMesh &manNavMesh, ManBoxPowerUp &manBoxPowerUp, ManTotem &manTotem) {
    StateInGame::InitializeSystems(manCars, manWall, manOBB, manGround, manPowerUp, manNavMesh, manBoxPowerUp, manTotem);
}

void StateInGameMulti::InitializeFacades() {
    StateInGame::InitializeFacades();
}

void StateInGameMulti::AddElementsToRender() {
    StateInGame::AddElementsToRender();
}
