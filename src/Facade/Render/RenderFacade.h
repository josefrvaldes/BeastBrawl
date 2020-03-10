#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <chrono>
#include <Entities/Entity.h>
#include <EventManager/EventManager.h>
#include <Managers/ManWayPoint.h>
#include <Managers/ManCar.h>
#include <Components/CPowerUp.h>
#include <Managers/Manager.h>
#include "../../Constants.h"

using namespace chrono;


class RenderFacade {
   public:
    //RenderFacade() = default;
    virtual ~RenderFacade() = default;
    virtual const uint16_t FacadeAddObject(Entity*) = 0;
    virtual void FacadeAddSphereOnObject(Entity* entity) = 0;
    virtual const uint16_t FacadeAddObjectCar(Entity*) = 0;
    virtual const uint16_t FacadeAddObjectTotem(Entity* entity) = 0;
    virtual const void FacadeAddObjects(vector<Entity*>) = 0;
    virtual void FacadeAddCamera(Entity*) = 0;
    virtual void UpdateCamera(Entity*, ManCar* manCars) = 0;
    virtual bool FacadeRun() = 0;
    virtual uint32_t FacadeGetTime() const = 0;
    virtual vector<Constants::InputTypes> FacadeCheckInputMulti() = 0;
    virtual void FacadeCheckInputSingle() = 0;
    virtual int FacadeGetFPS() const = 0;
    virtual void FacadeSetWindowCaption(std::string, int) const = 0;
    virtual void FacadeBeginScene() const = 0;
    virtual void FacadeDrawAll() const = 0;
    virtual void FacadeEndScene() const = 0;
    virtual void FacadeDeviceDrop() = 0;
    virtual void FacadeDraw() const = 0;
    virtual void DeleteEntity(Entity*) = 0;
    virtual void FacadeDrawMenu() = 0;
    virtual void FacadeDrawPause() = 0;
    virtual void FacadeDrawEndRace() = 0;
    virtual void FacadeDrawLobbyMulti() = 0;
    virtual void FacadeDrawLobbyMultiExit() = 0;
    virtual void FacadeDrawControler() = 0;
    virtual void FacadeInitMenu() = 0;
    virtual void FacadeInitPause() = 0;
    virtual void FacadeInitEndRace() = 0;
    virtual void FacadeInitLobbyMulti() = 0;
    virtual void FacadeInitControler() = 0;
    virtual void FacadeInitHUD() = 0;
    virtual void FacadeCheckInputMenu() = 0;
    virtual void FacadeCheckInputPause() = 0;
    virtual void FacadeCheckInputEndRace() = 0;
    virtual void FacadeCheckInputLobbyMulti() = 0;
    virtual void FacadeCheckInputControler() = 0;
    virtual void FacadeUpdatePowerUpHUD(DataMap* d) = 0;
    virtual void FacadeDrawHUD(Entity* car, ManCar* manCars) = 0;
    virtual void FacadeSuscribeEvents() = 0;
    virtual void FacadeAddPlates(Manager* manNamePlates) = 0;
    virtual void FacadeUpdatePlates(Manager* manNamePlates) = 0;
    virtual void ThrowEventChangeToMulti(uint16_t IdOnline, const vector<uint16_t> IdPlayersOnline) = 0;

    //DEBUG
    virtual void Draw3DLine(vec3 &pos1, vec3 &pos2, uint16_t r, uint16_t g, uint16_t b) const = 0;
    virtual void Draw3DLine(vec3 &pos1, vec3 &pos2) const = 0;
    virtual void FacadeDrawGraphEdges(ManWayPoint* manWayPoints) const = 0;
    virtual void FacadeDrawBoundingBox(Entity* entity, bool colliding) const = 0;
    virtual void FacadeDrawBoundingPlane(Entity* entity) const = 0;
    virtual void FacadeDrawBoundingGround(Entity* entity) const = 0;
    virtual void FacadeDrawBoundingOBB(Entity* entity) const = 0;
    virtual void FacadeDrawAIDebug(ManCar* manCars, ManNavMesh* manNavMesh, ManWayPoint* manWayPoint) const = 0;
    virtual void FacadeDrawAIDebugPath(Entity* carAI, ManWayPoint* manWayPoint) const = 0;

    virtual void SetShowDebug(bool) = 0;
    virtual void SetShowDebugAI(bool) = 0;
    virtual void SetIDCarAIToDebug(int) = 0;

    virtual bool GetShowDebug() = 0;
    virtual bool GetShowDebugAI() = 0;
    virtual int  GetIDCarAIToDebug() = 0;

   protected:
    uint16_t idCar = 0;
    uint16_t idTotem = 0;
    uint16_t idCarWithTotem = 0;
    int numEnemyCars = 0;
    int currentPowerUp = 0;


    time_point<system_clock> timeStart;
    const int64_t inputDelay = 500;       // 0.5 segundos
    const int64_t inputDelayCamera = 250;       // 0.5 segundos
    bool invertedCam = false;
    bool totemCamActive = false;
    int idCarAIToDebug = 0;
};
