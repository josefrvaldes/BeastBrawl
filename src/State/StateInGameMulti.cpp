#include "StateInGameMulti.h"
#include <Components/CTotem.h>
#include "../Systems/SystemOnline.h"
#include "../Components/COnline.h"

StateInGameMulti::StateInGameMulti() : StateInGame() {
    InitVirtualMethods();
    vec3 pos = vec3(120.0f, 20.0f, -300.0f);
    manCars->CreateHumanCar(pos);

    shared_ptr<Entity> car1 = manCars->GetEntities()[0];
    COnline *cOnline1 = static_cast<COnline *>(car1->GetComponent(CompType::OnlineComp).get());
    cOnline1->idClient = 1;

    shared_ptr<Entity> car2 = manCars->GetEntities()[1];
    COnline *cOnline2 = static_cast<COnline *>(car2->GetComponent(CompType::OnlineComp).get());
    cOnline2->idClient = 2;

    renderEngine->FacadeAddObject(car2.get());

    vector<Constants::InputTypes> inputs;
    sysOnline->SendInputs(inputs);  // enviamos un vector vacío la primera vez para que el servidor sepa que estamos vivos
}

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
        vec3(-50.0f, 10.0f, -50.0f)};

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
    }
    vector<Constants::InputTypes> inputs;
    sysOnline->SendInputs(inputs);  // enviamos un vector vacío la primera vez para que el servidor sepa que estamos vivos
}

StateInGameMulti::~StateInGameMulti() {
}

void StateInGameMulti::InitState() {
    StateInGame::InitState();
}

void StateInGameMulti::Input() {
    vector<Constants::InputTypes> inputs = renderEngine->FacadeCheckInputMulti();

    time_point<system_clock> now = system_clock::now();
    auto millisSinceLastInputSent = duration_cast<milliseconds>(now - lastTimeSentInputs).count();
    if (millisSinceLastInputSent > 66) {  // 100 = 10fps; 66 = 15fps   1000 = 60fps
        lastTimeSentInputs = now;
        sysOnline->SendInputs(inputs);
    }


    auto millisSinceLastSyncSent = duration_cast<milliseconds>(now - lastTimeSentSync).count();
    if (millisSinceLastSyncSent > 250) {  // 1000ms = 1s = 60fps; 2s = 120frames
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
            physicsEngine->UpdateCarAI(actualCar.get());
        }
    }

    CAMBIARCosasDeTotemUpdate();
    // COLISIONES entre powerUp y cocheHuman
    collisions->IntersectsCarsPowerUps(manCars.get(), manPowerUps.get(), manNavMesh.get());
    // COLISIONES entre BoxPowerUp y cocheHuman
    collisions->IntersectCarsBoxPowerUp(manCars.get(), manBoxPowerUps.get());
    // COLISIONES  entre la cocheHuman y el Totem
    collisions->IntersectCarsTotem(manCars.get(), manTotems.get());
    //if(manTotems->GetEntities().size() != 0){
    //    auto cTransformable = static_cast<CTransformable *>(manTotems->GetEntities()[0]->GetComponent(CompType::TransformableComp).get());
    //    std::cout << "X: " << cTransformable->position.x << "  -  Y: " << cTransformable->position.z << "\n";
    //}else{
    //    std::cout << "No existe totem\n";
    //}
}

void StateInGameMulti::Render() {
    renderEngine->FacadeDrawBoundingBox(manCars.get()->GetCar().get(), true);
    StateInGame::Render();
}

void StateInGameMulti::InitializeCLPhysics(ManCar &manCars, ManBoundingWall &manBoundingWall, ManBoundingOBB &manBoundingOBB) {
    StateInGame::InitializeCLPhysics(manCars, manBoundingWall, manBoundingOBB);
}

void StateInGameMulti::InitializeManagers(Physics *physics, Camera *cam) {
    StateInGame::InitializeManagers(physics, cam);
}

void StateInGameMulti::InitializeSystems(ManCar &manCars, ManBoundingWall &manBoundingWall, ManBoundingOBB &manBoundingOBB) {
    StateInGame::InitializeSystems(manCars, manBoundingWall, manBoundingOBB);
}

void StateInGameMulti::InitializeFacades() {
    StateInGame::InitializeFacades();
}

void StateInGameMulti::AddElementsToRender() {
    StateInGame::AddElementsToRender();
}

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
