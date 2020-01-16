#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <chrono>
#include "../../Entities/Entity.h"
#include "../../EventManager/EventManager.h"
#include "../../Managers/ManWayPoint.h"
#include "../../Managers/ManNavMesh.h"
#include "../../Managers/ManCar.h"
#include "../../Components/CPowerUp.h"
#include "../../Managers/Manager.h"

using namespace chrono;


class RenderFacade {
   public:
    //RenderFacade() = default;
    virtual ~RenderFacade() = default;
    virtual const uint16_t FacadeAddObject(Entity*) = 0;
    virtual const uint16_t FacadeAddObjectCar(Entity*) = 0;
    virtual const uint16_t FacadeAddObjectTotem(Entity* entity) = 0;
    virtual const void FacadeAddObjects(vector<Entity*>) = 0;
    virtual void FacadeAddCamera(Entity*) = 0;
    virtual void UpdateTransformable(Entity*) = 0;
    virtual void UpdateCamera(Entity*, ManCar* manCars) = 0;
    virtual bool FacadeRun() = 0;
    virtual uint32_t FacadeGetTime() = 0;
    virtual void FacadeCheckInput() = 0;
    virtual int FacadeGetFPS() = 0;
    virtual void FacadeSetWindowCaption(std::string) = 0;
    virtual void FacadeBeginScene() = 0;
    virtual void FacadeDrawAll() = 0;
    virtual void FacadeEndScene() = 0;
    virtual void FacadeDeviceDrop() = 0;
    virtual void FacadeDraw() = 0;
    virtual void DeleteEntity(Entity*) = 0;
    virtual void FacadeDrawMenu() = 0;
    virtual void FacadeDrawPause() = 0;
    virtual void FacadeDrawEndRace() = 0;
    virtual void FacadeInitMenu() = 0;
    virtual void FacadeInitPause() = 0;
    virtual void FacadeInitEndRace() = 0;
    virtual void FacadeInitHUD() = 0;
    virtual void FacadeCheckInputMenu() = 0;
    virtual void FacadeCheckInputPause() = 0;
    virtual void FacadeCheckInputEndRace() = 0;
    virtual void FacadeUpdatePowerUpHUD(DataMap d) = 0;
    virtual void FacadeDrawHUD(Entity* car, ManCar* carsAI) = 0;
    virtual void FacadeSuscribeEvents() = 0;
    virtual void FacadeAddPlates(Manager* manNamePlates) = 0;
    virtual void FacadeUpdatePlates(Manager* manNamePlates) = 0;

    //DEBUG
    virtual void Draw3DLine(vec3 &pos1, vec3 &pos2, uint16_t r, uint16_t g, uint16_t b) const = 0;
    virtual void Draw3DLine(vec3 &pos1, vec3 &pos2) const = 0;
    virtual void FacadeDrawGraphEdges(ManWayPoint* manWayPoints) = 0;
    virtual void FacadeDrawBoundingBox(Entity* entity, bool colliding) = 0;
    virtual void FacadeDrawBoundingPlane(Entity* entity) const = 0;
    virtual void FacadeDrawAIDebug(ManCar* manCars, ManNavMesh* manNavMesh, ManWayPoint* manWayPoint) = 0;
    virtual void FacadeDrawAIDebugPath(CarAI* carAI, ManNavMesh* manNavMesh, ManWayPoint* manWayPoint) = 0;

   protected:
    uint16_t idCar = 0;
    uint16_t idTotem = 0;
    uint16_t idCarWithTotem = 0;
    int numEnemyCars = 0;

    time_point<system_clock> timeStart;
    const int64_t inputDelay = 500;       // 0.5 segundos
    const int64_t inputDelayCamera = 250;       // 0.5 segundos
    bool invertedCam = false;
    bool totemCamActive = false;
    int idCarAIToDebug = 0;
};
