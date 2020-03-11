#include "StateInGameMulti.h"
#include <Components/CBufferOnline.h>
#include <Components/CTotem.h>
#include "../Components/COnline.h"
#include "../Systems/SystemOnline.h"
#include "../Systems/Utils.h"
#include "../CLPhysics/CLPhysics.h"

StateInGameMulti::StateInGameMulti(uint16_t IdOnline, const vector<uint16_t> IdPlayersOnline) : StateInGame() {
    InitState();
    InitVirtualMethods();
    // a este le llegan los coches
    //std::cout << "POR FIIIIIIIIIIIIIIIIIIIIIIIN: " << std::endl;
    vector<uint16_t> arrayIdEnemies = IdPlayersOnline;

    sysOnline = make_unique<SystemOnline>(*manCars, IdOnline);
    manCars->SetSystemOnline(sysOnline.get());
    manTotems->SetSystemOnline(sysOnline.get());
    manPowerUps->SetSystemOnline(sysOnline.get());
    clPhysics->SetSystemOnline(sysOnline.get());

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

    //CAMBIARCosasNavMesh(*manCars.get(), *manNavMesh.get());
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
        //cout << Utils::getISOCurrentTimestampMillis() << " [" << sysOnline->idOnlineMainCar << "] Enviamos los inputs porque han cambiado con respecto a la iteración anterior" << endl;
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
            //manNavMesh->UpdateNavMeshHuman(actualCar.get());  // actualizamos el navemesh en el que se encuentra al human
            // funcion para recibir los inputs del servidor, otra para enviar los nuestros, crear componente de input
            physics->UpdateHuman(static_cast<Car *>(actualCar.get()));
            manCars->UpdateCarHuman(actualCar.get(), manTotems.get());
            physicsEngine->UpdateTransformable(actualCar.get());
        }
    }
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

/*
void StateInGameMulti::CAMBIARCosasDeTotemUpdate() {
    bool todosFalse = true;
    auto cTransformTotem = static_cast<CTransformable *>(totemOnCar.get()->GetComponent(CompType::TransformableComp).get());
    cTransformTotem->rotation.y += 0.1;
    for (auto currentCar : manCars->GetEntities()) {  // actualizamos los coche IA
        // comprobamos el componente totem y si lo tienen se lo ponemos justo encima para que se sepa quien lo lleva
        auto cTotem = static_cast<CTotem *>(currentCar.get()->GetComponent(CompType::TotemComp).get());
        if (cTotem->active) {
            todosFalse = false;
            auto cTransformCar = static_cast<CTransformable *>(currentCar.get()->GetComponent(CompType::TransformableComp).get());
            cTransformTotem->position.x = cTransformCar->position.x;
            cTransformTotem->position.z = cTransformCar->position.z;
            cTransformTotem->position.y = 32.0f;
            // supuestamente esta el drawAll que te lo hace no?????????????????
            // si esta cambiando pero no se esta redibujando
            break;  // cuando encontramos a alguien que ya lleva el totem, nos salimos del for, no seguimos comprobando a los demás
        }
    }
    if (todosFalse) {
        cTransformTotem->position.y = -100.0f;
    }

    renderEngine->UpdateTransformable(totemOnCar.get());
}
*/
