#pragma once

#include <chrono>
#include <cstdint>
#include <iostream>
#include <list>
#include <memory>
#include <queue>
#include <stack>
#include <string>

#include <glm/vec3.hpp>
#include <Entities/BoxPowerUp.h>
#include <Entities/Camera.h>
#include <Entities/Car.h>
#include <Entities/CarHuman.h>
#include <Entities/CarAI.h>
#include <Entities/GameObject.h>
#include <Entities/PowerUp.h>
#include <Entities/Totem.h>
#include <Entities/WayPoint.h>
#include <Entities/NavMesh.h>
#include <Managers/ManPowerUp.h>
#include <Managers/ManWayPoint.h>
#include <Managers/ManNavMesh.h>
#include <EventManager/EventManager.h>
#include <Facade/Input/InputFacadeManager.h>
#include <Facade/Physics/PhysicsFacadeManager.h>
#include <Facade/Render/RenderFacadeManager.h>
#include <Facade/Sound/SoundFacade.h>
#include <Facade/Sound/SoundFacadeManager.h>
#include <Systems/SystemPathPlanning.h>
#include <Game.h>
#include <Managers/ManBoundingWall.h>
#include <Managers/ManBoundingGround.h>
#include <Managers/ManBoxPowerUp.h>
#include <Managers/ManCar.h>
#include <Managers/ManNamePlate.h>
#include <Managers/ManPowerUp.h>
#include <Managers/ManTotem.h>
#include <Managers/ManWayPoint.h>
#include <Systems/Collisions.h>
#include <Systems/Physics.h>
#include <Systems/PhysicsPowerUp.h>
#include <Systems/SystemBoxPowerUp.h>
#include <behaviourTree/behaviourTree.h>
#include <behaviourTree/decorator.h>
#include <behaviourTree/selector.h>
#include <behaviourTree/sequence.h>
#include <fuzzyLogic/fuzzyLogic.h>
#include <Components/CNavMesh.h>
#include <Components/CCurrentNavMesh.h>
#include "../Managers/ManBoundingOBB.h"
#include "../Constants.h"


using namespace std;
using namespace chrono;

class CLPhysics;

class StateInGame : public State {
   public:
    StateInGame();
    ~StateInGame();
    void InitVirtualMethods();
    void InitState() override;
    virtual void Input() = 0;
    void Update() override;
    void Render() override;
    States GetState() { return State::States::INGAME_SINGLE; };

    shared_ptr<ManCar> manCars;

   protected:
    shared_ptr<GameObject> ground;
    shared_ptr<Camera> cam;
    shared_ptr<ManPowerUp> manPowerUps;
    shared_ptr<ManBoxPowerUp> manBoxPowerUps;
    shared_ptr<ManNavMesh> manNavMesh;
    shared_ptr<ManWayPoint> manWayPoint;
    shared_ptr<ManNamePlate> manNamePlates;
    shared_ptr<ManBoundingWall> manBoundingWall;
    shared_ptr<ManBoundingOBB> manBoundingOBB;
    shared_ptr<ManBoundingGround> manBoundingGround;

    RenderFacade *renderEngine = {nullptr};
    InputFacade *inputEngine = {nullptr};
    PhysicsFacade *physicsEngine = {nullptr};
    SoundFacade *soundEngine = {nullptr};

    shared_ptr<Physics> physics;
    //shared_ptr<float> deltaTime;
    
    shared_ptr<PhysicsPowerUp> phisicsPowerUp;
    shared_ptr<SystemBoxPowerUp> sysBoxPowerUp;
    shared_ptr<Collisions> collisions;
    shared_ptr<Totem> totem;
    shared_ptr<Entity> totemOnCar;

    unique_ptr<CLPhysics> clPhysics;
    // unique_ptr<SystemPathPlanning> sysPathPlanning;

    shared_ptr<ManTotem> manTotems;
    //int lastFPS = -1;
    //uint32_t then;
    time_point<system_clock> then;
    // vector<float> deltas;
    //float CalculateDelta(float);

    virtual void InitializeCLPhysics(ManCar&, ManBoundingWall&, ManBoundingOBB&, ManBoundingGround&, ManPowerUp&, ManNavMesh&, ManBoxPowerUp&, ManTotem &);
    virtual void InitializeManagers(Physics *, Camera *);
    virtual void InitializeSystems(ManCar&, ManBoundingWall&, ManBoundingOBB&, ManBoundingGround&, ManPowerUp&, ManNavMesh&, ManBoxPowerUp&, ManTotem &);
    virtual void InitializeFacades();
    virtual void AddElementsToRender();

    void CAMBIARCosasDeTotem(ManTotem &);
    void CAMBIARCosasNavMesh(ManCar &, ManNavMesh &);
    void CAMBIARPositionTotemAboveCar();

};
