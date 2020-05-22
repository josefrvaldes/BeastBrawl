#pragma once

#include "StateInGame.h"

#include "../Systems/SystemBtPowerUp.h"
#include "../Systems/SystemBtMoveTo.h"
#include "../Systems/SystemBtLoDMove.h"
#include "../Systems/SystemVisionAI.h"
#include "../Systems/SystemBtDecisionMove.h"

#include "../Managers/ManAI.h"

class StateInGameSingle : public StateInGame {
   public:
    StateInGameSingle();
    ~StateInGameSingle();
    void InitState() override;
    void Input() override;
    bool UpdateAnimationStart() override;
    void UpdateAnimationCountdown() override;
    void UpdateAnimationEnd() override;
    void UpdateGame() override;
    void Update() override;
    void Render() override;
    States GetState() override { return State::States::INGAME_SINGLE; };

   private:
    void InitializeCLPhysics(ManCar &manCars, ManBoundingWall &, ManBoundingOBB &, ManBoundingGround &, ManPowerUp &, ManNavMesh&, ManBoxPowerUp&, ManTotem &) override;
    void InitializeManagers() override;
    void InitializeSystems(ManCar &manCars, ManBoundingWall &, ManBoundingOBB &, ManBoundingGround &, ManPowerUp &, ManNavMesh&, ManBoxPowerUp&, ManTotem &) override;
    void InitializeFacades() override;
    void AddElementsToRender() override;

    void InitCarAIS(ManCar &, ManWayPoint &);
    //void CAMBIARCosasNavMesh(ManCar &, ManNavMesh &);
    //void CAMBIARCosasDeTotemUpdate() override;

    void createSystemAI();
    void InitBtPowerUp();
    void InitBtMoveTo();
    void InitBtLoDMove();
    void InitPathPlanning();
    void InitVision();
    void InitBtDecisionMove();

    unique_ptr<ManAI> manAI;
    unique_ptr<SystemBtPowerUp> systemBtPowerUp;
    unique_ptr<SystemBtMoveTo> systemBtMoveTo;
    unique_ptr<SystemBtLoDMove> systemBtLoDMove;
    unique_ptr<SystemPathPlanning> systemPathPlanning;
    unique_ptr<SystemVisionAI> systemVisionAI;
    unique_ptr<SystemBtDecisionMove> systemBtDecisionMove;

    bool goingToPause {false};
    bool comingBackFromPause {false};

    int64_t timeStartPause;


    std::chrono::time_point<std::chrono::system_clock> timeStart;        // reloj que contabiliza el tiempo
    std::chrono::time_point<std::chrono::system_clock> timeStartSeccion;        // reloj que contabiliza el tiempo
    double accumulatedTimeUPDATE {0.0};
    double accumulatedTimeRENDER {0.0};

};
