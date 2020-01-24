#include "StateInGameMulti.h"

#include "../Components/CTotem.h"
#include "../Components/COnline.h"
#include "../Systems/SystemOnline.h"

StateInGameMulti::StateInGameMulti() : StateInGame() {
    InitVirtualMethods();
    vec3 pos = vec3(120.0f, 20.0f, -300.0f);
    manCars->CreateHumanCar(pos);
    
    shared_ptr<Entity> car1 = manCars->GetEntities()[0];
    COnline* cOnline1 = static_cast<COnline*>(car1->GetComponent(CompType::OnlineComp).get());
    cOnline1->idClient = 1;
    
    shared_ptr<Entity> car2 = manCars->GetEntities()[1];
    COnline* cOnline2 = static_cast<COnline*>(car2->GetComponent(CompType::OnlineComp).get());
    cOnline2->idClient = 2;
    
    renderEngine->FacadeAddObject(car2.get());
}

StateInGameMulti::StateInGameMulti(string data) : StateInGame() {
    InitVirtualMethods();
    // a este le llegan los coches
    std::cout << "POR FIIIIIIIIIIIIIIIIIIIIIIIN: " << data << std::endl;
    json receivedJSON = json::parse(data);
    uint32_t idPlayer = receivedJSON["idPlayer"];
    vector<uint32_t> arrayIdEnemies = receivedJSON["idEnemies"];

    auto cTransformable = static_cast<CTransformable*>(manCars->GetCar()->GetComponent(CompType::TransformableComp).get());
    if(idPlayer == 1){
        cTransformable->position = vec3(120.0f, 20.0f, -300.0f);
    }else if(idPlayer == 2){
        cTransformable->position = vec3(20.0f, 20.0f, -300.0f);
    }else if(idPlayer == 3){
        cTransformable->position = vec3(40.0f, 20.0f, -150.0f);
    }else if(idPlayer == 4){
        cTransformable->position = vec3(-50.0f, 20.0f, -50.0f);
    }

    for(auto idEnemy : arrayIdEnemies){
        if(idEnemy==1){
            vec3 pos = vec3(120.0f, 20.0f, -300.0f);
            manCars->CreateHumanCar(pos);
            shared_ptr<Entity> car = manCars->GetEntities()[manCars->GetEntities().size()-1];
            COnline* cOnline = static_cast<COnline*>(car->GetComponent(CompType::OnlineComp).get());
            cOnline->idClient = 1;
            renderEngine->FacadeAddObject(car.get());
        }else if(idEnemy == 2){
            vec3 pos = vec3(20.0f, 20.0f, -300.0f);
            manCars->CreateHumanCar(pos);
            shared_ptr<Entity> car = manCars->GetEntities()[manCars->GetEntities().size()-1];
            COnline* cOnline = static_cast<COnline*>(car->GetComponent(CompType::OnlineComp).get());
            cOnline->idClient = 2;
            renderEngine->FacadeAddObject(car.get());
        }else if(idEnemy == 3){
            vec3 pos = vec3(40.0f, 20.0f, -150.0f);
            manCars->CreateHumanCar(pos);
            shared_ptr<Entity> car = manCars->GetEntities()[manCars->GetEntities().size()-1];
            COnline* cOnline = static_cast<COnline*>(car->GetComponent(CompType::OnlineComp).get());
            cOnline->idClient = 3;
            renderEngine->FacadeAddObject(car.get());
        }else if(idEnemy == 4){
            vec3 pos = vec3(-50.0f, 20.0f, -50.0f);
            manCars->CreateHumanCar(pos);
            shared_ptr<Entity> car = manCars->GetEntities()[manCars->GetEntities().size()-1];
            COnline* cOnline = static_cast<COnline*>(car->GetComponent(CompType::OnlineComp).get());
            cOnline->idClient = 4;
            renderEngine->FacadeAddObject(car.get());
        }
    }
    
}

StateInGameMulti::~StateInGameMulti() {
}

void StateInGameMulti::InitState() {
    StateInGame::InitState();
}

void StateInGameMulti::Input() {
    vector<Constants::InputTypes> inputs = renderEngine->FacadeCheckInput();
    sysOnline->SendInputs(inputs);
    inputEngine->CheckInput();
}

void StateInGameMulti::Update() {
    StateInGame::Update();

    CAMBIARCosasDeTotemUpdate();

    for (auto actualCar : manCars->GetEntities()) {
        if (actualCar.get() != manCars->GetCar().get()) {
            // funcion para recibir los inputs del servidor, otra para enviar los nuestros, crear componente de input
            physics->UpdateHuman(static_cast<Car *>(actualCar.get()));
            manCars->UpdateCarHuman(actualCar.get());
            physicsEngine->UpdateCarAI(actualCar.get());
        }
    }

    // COLISIONES entre powerUp y cocheHuman
    collisions->IntersectsCarsPowerUps(manCars.get(), manPowerUps.get(), manNavMesh.get());
    // COLISIONES entre BoxPowerUp y cocheHuman
    collisions->IntersectCarsBoxPowerUp(manCars.get(), manBoxPowerUps.get());
    // COLISIONES  entre la cocheHuman y el Totem
    collisions->IntersectCarsTotem(manCars.get(), manTotems.get());
}

void StateInGameMulti::Render() {
    renderEngine->FacadeDrawBoundingBox(manCars.get()->GetCar().get(), true);
    StateInGame::Render();
}

void StateInGameMulti::InitializeCLPhysics(ManCar &manCars, ManBoundingWall &manBoundingWall) {
    StateInGame::InitializeCLPhysics(manCars, manBoundingWall);
}

void StateInGameMulti::InitializeManagers(Physics *physics, Camera *cam) {
    StateInGame::InitializeManagers(physics, cam);
}

void StateInGameMulti::InitializeSystems(ManCar &manCars, ManBoundingWall &manBoundingWall) {
    StateInGame::InitializeSystems(manCars, manBoundingWall);
    sysOnline = make_unique<SystemOnline>(manCars);
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
    for (shared_ptr<Entity> currentCar : manCars->GetEntities()) {  // actualizamos los coche IA
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
