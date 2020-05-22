/**
 * Beast Brawl
 * Game created as a final project of the Multimedia Engineering Degree in the University of Alicante.
 * Made by Clover Games Studio, with members 
 * Carlos de la Fuente Torres delafuentetorresc@gmail.com,
 * Antonio Jose Martinez Garcia https://www.linkedin.com/in/antonio-jose-martinez-garcia/,
 * Jesús Mas Carretero jmasc03@gmail.com, 
 * Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/, 
 * Rubén Rubio Martínez https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/, 
 * and Jose Valdés Sirvent https://www.linkedin.com/in/jose-f-valdés-sirvent-6058b5a5/ github -> josefrvaldes
 * 
 * 
 * @author Clover Games Studio
 * 
 */
 
 
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
#include <EventManager/EventManager.h>
#include <Facade/Input/InputFacadeManager.h>
#include <Facade/Physics/PhysicsFacadeManager.h>
#include <Facade/Render/RenderFacadeManager.h>
#include <Facade/Sound/SoundFacade.h>
#include <Facade/Sound/SoundFacadeManager.h>
#include <Systems/SystemPathPlanning.h>
#include <Game.h>
#include <Managers/ManPowerUp.h>
#include <Managers/ManWayPoint.h>
#include <Managers/ManNavMesh.h>
#include <Managers/ManBoundingWall.h>
#include <Managers/ManBoundingGround.h>
#include <Managers/ManBoxPowerUp.h>
#include <Managers/ManCar.h>
#include <Managers/ManNamePlate.h>
#include <Managers/ManPowerUp.h>
#include <Managers/ManTotem.h>
#include <Managers/ManWayPoint.h>
#include <Managers/ManLight.h>
#include <Managers/ManParticleSystem.h>
#include <Managers/ManShield.h>
#include <Managers/ManCamera.h>
#include <Managers/ManSpawn.h>
#include "../Managers/ManGameRules.h"
#include <Managers/ManHUDEvent.h>
#include <Systems/Collisions.h>
#include <Systems/Physics.h>
#include <Systems/PhysicsPowerUp.h>
#include <Systems/SystemLoD.h>
#include <Systems/SystemAnimationStart.h>
#include <Systems/SystemAnimationEnd.h>
#include <Systems/SystemBoxPowerUp.h>
#include <Systems/SystemRanking.h>
#include <Systems/SystemHurt.h>
#include <Systems/Utils.h>
#include <Systems/SystemData.h>
#include <Systems/SysHud.h>
#include <behaviourTree/behaviourTree.h>
#include <behaviourTree/decorator.h>
#include <behaviourTree/selector.h>
#include <behaviourTree/sequence.h>
#include <fuzzyLogic/fuzzyLogic.h>
#include <Components/CNavMesh.h>
#include <Components/CCurrentNavMesh.h>
#include "../Managers/ManBoundingOBB.h"
#include "../Constants.h"
#include "../Octree/Octree.h"



using namespace std;
using namespace chrono;

class CLPhysics;

enum UpdateState {
    START,
    COUNTDOWN,
    GAME,
    WAITING_FOR_COUNTDOWN,
    END
};

class StateInGame : public State {
   public:
    StateInGame();
    ~StateInGame();
    
    void InitState() override;
    virtual void Input() = 0;
    void Update() override;
    virtual bool UpdateAnimationStart();
    virtual void UpdateAnimationCountdown();
    virtual void UpdateAnimationEnd();
    virtual void UpdateGame();
    void Render() override;
    States GetState() override { return State::States::INGAME_SINGLE; };
    void CreateMainCar();

    shared_ptr<ManCar> manCars;

   protected:
    unique_ptr<ManCamera> manCamera;
    shared_ptr<ManPowerUp> manPowerUps;
    shared_ptr<ManBoxPowerUp> manBoxPowerUps;
    shared_ptr<ManNavMesh> manNavMesh;
    shared_ptr<ManWayPoint> manWayPoint;
    shared_ptr<ManNamePlate> manNamePlates;
    shared_ptr<ManBoundingWall> manBoundingWall;
    shared_ptr<ManBoundingOBB> manBoundingOBB;
    shared_ptr<ManBoundingGround> manBoundingGround;
    shared_ptr<ManLight> manLight;
    unique_ptr<ManGameRules> manGameRules;
    unique_ptr<ManHUDEvent> manHudEvent;
    unique_ptr<ManParticleSystem> manParticleSystem;
    unique_ptr<ManShield> manShield;
    unique_ptr<ManSpawn> manSpawn;

    unique_ptr<SystemLoD> sysLoD;
    unique_ptr<SystemAnimationStart> sysAnimStart;
    unique_ptr<SystemAnimationEnd> sysAnimEnd;
    unique_ptr<SystemData> systemDataVision;
    unique_ptr<SystemHurt> sysHurt;
    unique_ptr<SysHud> sysHud;

    std::vector<shared_ptr<Manager>> managersEntities;
    std::unique_ptr<Octree> octreeScene;

    RenderFacade *renderEngine = {nullptr};
    InputFacade *inputEngine = {nullptr};
    PhysicsFacade *physicsEngine = {nullptr};
    SoundFacade *soundEngine = {nullptr};

    //shared_ptr<Physics> physics;
    //shared_ptr<float> deltaTime;
    
    shared_ptr<SystemRanking> sysRanking;
    shared_ptr<SystemBoxPowerUp> sysBoxPowerUp;
    shared_ptr<Collisions> collisions;
    shared_ptr<Totem> totem;
    //shared_ptr<Entity> totemOnCar;

    unique_ptr<CLPhysics> clPhysics;
    // unique_ptr<SystemPathPlanning> sysPathPlanning;

    shared_ptr<ManTotem> manTotems;
    //int lastFPS = -1;
    //uint32_t then;
    time_point<system_clock> then;
    // vector<float> deltas;
    //float CalculateDelta(float);

    virtual void InitializeCLPhysics(ManCar&, ManBoundingWall&, ManBoundingOBB&, ManBoundingGround&, ManPowerUp&, ManNavMesh&, ManBoxPowerUp&, ManTotem &);
    virtual void InitializeManagers();
    virtual void InitializeSystems(ManCar&, ManBoundingWall&, ManBoundingOBB&, ManBoundingGround&, ManPowerUp&, ManNavMesh&, ManBoxPowerUp&, ManTotem &);
    virtual void InitializeFacades();
    virtual void AddElementsToRender();
    void GoToEndAnimation();
    void GoToStateEndrace();
    virtual void GoToUpdateGame();
    void GoToCountdownAnimation();
    void InitializeSystemData();
    //virtual void CAMBIARCosasDeTotemUpdate(){};

    void IntersectsCLPhysics();

    void CreateVegetation();
    
    //void CAMBIARCosasDeTotem(ManTotem &);
    //void CAMBIARCosasNavMesh(ManCar &, ManNavMesh &);
    //void CAMBIARPositionTotemAboveCar();
    UpdateState currentUpdateState {UpdateState::START};
    int64_t timerCountdown;
    uint8_t currentCountdown{3};
    int64_t timerEnd {0};



    std::chrono::time_point<std::chrono::system_clock> timeStart;        // reloj que contabiliza el tiempo
    std::chrono::time_point<std::chrono::system_clock> timeStartSeccion;        // reloj que contabiliza el tiempo
    double accumulatedTimeUPDATE {0.0};



};
