#pragma once

#include "StateInGame.h"

#include "../Systems/SystemBtPowerUp.h"
#include "../Systems/SystemBtMoveTo.h"
#include "../Systems/SystemBtLoDMove.h"

#include "../Managers/ManAI.h"

class StateInGameSingle : public StateInGame {
   public:
    StateInGameSingle();
    ~StateInGameSingle();
    void InitState() override;
    void Input() override;
    void UpdateAnimationStart() override;
    void UpdateAnimationCountdown() override;
    void UpdateAnimationEnd() override;
    void UpdateGame() override;
    void Update() override;
    void Render() override;
    States GetState() override { return State::States::INGAME_SINGLE; };

   private:
    void InitializeCLPhysics(ManCar &manCars, ManBoundingWall &, ManBoundingOBB &, ManBoundingGround &, ManPowerUp &, ManNavMesh&, ManBoxPowerUp&, ManTotem &) override;
    void InitializeManagers(const uint32_t timeGame) override;
    void InitializeSystems(ManCar &manCars, ManBoundingWall &, ManBoundingOBB &, ManBoundingGround &, ManPowerUp &, ManNavMesh&, ManBoxPowerUp&, ManTotem &) override;
    void InitializeFacades() override;
    void AddElementsToRender() override;

    void CAMBIARInicializarCarAIS(ManCar &, ManWayPoint &);
    //void CAMBIARCosasNavMesh(ManCar &, ManNavMesh &);
    //void CAMBIARCosasDeTotemUpdate() override;

    void createSystemAI();
    void InitBtPowerUp();
    void InitBtMoveTo();
    void InitBtLoDMove();
    void InitPathPlanning();

    unique_ptr<ManAI> manAI;
    unique_ptr<SystemBtPowerUp> systemBtPowerUp;
    unique_ptr<SystemBtMoveTo> systemBtMoveTo;
    unique_ptr<SystemBtLoDMove> systemBtLoDMove;
    unique_ptr<SystemPathPlanning> systemPathPlanning;


    bool goingToPause {false};
    bool comingBackFromPause {false};
};
