#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include "../../Entities/Entity.h"
#include "../../EventManager/EventManager.h"
#include "../../Managers/ManWayPoint.h"

class GameObject;
class RenderFacade {
   public:
    //RenderFacade() = default;
    virtual ~RenderFacade() = default;
    virtual const uint16_t FacadeAddObject(Entity*) = 0;
    virtual const uint16_t FacadeAddObjectCar(Entity*) = 0;
    virtual const void FacadeAddObjects(vector<Entity*>) = 0;
    virtual void FacadeAddCamera(Entity*) = 0;
    virtual void UpdateTransformable(Entity*) = 0;
    virtual void UpdateCamera(Entity*) = 0;
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

    //DEBUG
    virtual void FacadeDrawGraphEdges(ManWayPoint* manWayPoints) = 0;
    virtual void FacadeDrawBoundingBox(Entity* entity, bool colliding) = 0;

   protected:
    uint16_t idCar = 0;
};
