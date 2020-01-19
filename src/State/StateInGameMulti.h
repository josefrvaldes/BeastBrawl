#pragma once

#include "StateInGame.h"

class StateInGameMulti : public StateInGame {
   public:
    StateInGameMulti();
    ~StateInGameMulti();
    void InitState() override;
    void Input() override;
    void Update() override;
    void Render() override;
    States GetState() { return State::States::INGAME_MULTI; };

   private:
    void InitializeCLPhysics(ManCar &manCars, ManBoundingWall &ManBoundingWall) override;
    void InitializeManagers(Physics *physics, Camera *cam) override;
    void InitializeSystems(ManCar &manCars, ManBoundingWall &manBoundingWall) override;
    void InitializeFacades() override;
    void AddElementsToRender() override;

    void CAMBIARCosasDeTotemUpdate() override;

};
