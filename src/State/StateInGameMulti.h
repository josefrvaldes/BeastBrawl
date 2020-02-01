#pragma once

#include "StateInGame.h"
#include <chrono>

using namespace std::chrono;

class SystemOnline;

class StateInGameMulti : public StateInGame {
   public:
    StateInGameMulti();
    StateInGameMulti(string data);
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

    unique_ptr<SystemOnline> sysOnline;

    time_point<system_clock> lastTimeSentInputs;
};
