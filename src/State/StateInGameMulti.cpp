#include "StateInGameMulti.h"
#include "../Components/CTotem.h"

StateInGameMulti::StateInGameMulti() : StateInGame() {
    InitVirtualMethods();
}

StateInGameMulti::~StateInGameMulti() {
}

void StateInGameMulti::InitState() {
    StateInGame::InitState();
}

void StateInGameMulti::Input() {
    StateInGame::Input();
}

void StateInGameMulti::Update() {
    StateInGame::Update();

    CAMBIARCosasDeTotemUpdate();
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
            break; // cuando encontramos a alguien que ya lleva el totem, nos salimos del for, no seguimos comprobando a los demás
        }
    }
    if (todosFalse) {
        cTransformTotem->position.y = -100.0f;
    }

    renderEngine->UpdateTransformable(totemOnCar.get());
}
