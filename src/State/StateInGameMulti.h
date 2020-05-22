#pragma once

#include <chrono>
#include "StateInGame.h"
#include "../Aliases.h"


using namespace std::chrono;

class SystemOnline;

class StateInGameMulti : public StateInGame {
   public:
    StateInGameMulti(uint16_t idOnline_, const vector<uint16_t> idsEnemies_, const vector<uint8_t> characters_);
    ~StateInGameMulti();
    void InitState() override;
    void Input() override;
    void Update() override;
    bool UpdateAnimationStart() override;
    void UpdateAnimationCountdown() override;
    void UpdateAnimationEnd() override;
    void UpdateWaitingForCountdown();
    void UpdateGame() override;
    void Render() override;
    States GetState() override { return State::States::INGAME_MULTI; };

   private:
    void SubscribeToEvents();
    void GoToEndAnimationFromMulti(DataMap *);
    void GoToCountdownAnimationFromMulti(DataMap *);
    void GoToUpdateGame() override;
    void InitializeCLPhysics(ManCar &manCars, ManBoundingWall &, ManBoundingOBB &, ManBoundingGround &, ManPowerUp &, ManNavMesh&, ManBoxPowerUp&, ManTotem &) override;
    void InitializeManagers() override;
    void InitializeSystems(ManCar &manCars, ManBoundingWall &, ManBoundingOBB &, ManBoundingGround &, ManPowerUp &, ManNavMesh&, ManBoxPowerUp&, ManTotem &) override;
    void InitializeFacades() override;
    void AddElementsToRender() override;
    void InitCarHumans(const uint16_t idOnline_, const vector<uint16_t> idsEnemies_, const vector<uint8_t> characters_);
    //void CAMBIARCosasDeTotemUpdate() override;

    unique_ptr<SystemOnline> sysOnline;

    time_point<system_clock> lastTimeSentInputs;
    time_point<system_clock> lastTimeSentSync;
    vector<Constants::InputTypes> previousInputs;
    bool readyToCountdown{false};
};
