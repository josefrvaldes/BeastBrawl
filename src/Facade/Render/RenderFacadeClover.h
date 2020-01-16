#pragma once

#include "RenderFacade.h"
#include "../../EventManager/Event.h"
#include "../../EventManager/EventManager.h"

#include <codecvt>
#include <iostream>
#include <locale>
#include <memory>
#include <string>
#include <unordered_map>

   

using namespace std;

class RenderFacadeClover : public RenderFacade {
   public:
      RenderFacadeClover();
      ~RenderFacadeClover() override;
      const uint16_t FacadeAddObject(Entity*) override;
      const uint16_t FacadeAddObjectCar(Entity*) override;
      const uint16_t FacadeAddObjectTotem(Entity* entity) override;
      const void FacadeAddObjects(vector<Entity*>) override;
      void FacadeAddCamera(Entity*) override;
      void UpdateTransformable(Entity*) override;
      void UpdateCamera(Entity*, ManCar* manCars) override;
      bool FacadeRun() override;
      uint32_t FacadeGetTime() override;
      void FacadeCheckInput() override;
      int FacadeGetFPS() override;
      void FacadeSetWindowCaption(std::string) override;
      void FacadeBeginScene() override;
      void FacadeDrawAll() override;
      void FacadeEndScene() override;
      void FacadeDeviceDrop() override;
      void FacadeDraw() override;
      void DeleteEntity(Entity*) override;
      void FacadeDrawMenu() override;
      void FacadeDrawPause() override;
      void FacadeDrawEndRace() override;
      void FacadeInitMenu() override;
      void FacadeInitPause() override;
      void FacadeInitEndRace() override;
      void FacadeInitHUD() override;
      void FacadeCheckInputMenu() override;
      void FacadeCheckInputPause() override;
      void FacadeCheckInputEndRace() override;
      void FacadeUpdatePowerUpHUD(DataMap d) override;
      void FacadeDrawHUD(Entity* car, ManCar* carsAI) override;
      void FacadeSuscribeEvents() override;
      void FacadeAddPlates(Manager* manNamePlates) override;
      void FacadeUpdatePlates(Manager* manNamePlates) override;

      //DEBUG
      void Draw3DLine(vec3& pos1, vec3& pos2, uint16_t r, uint16_t g, uint16_t b) const override;
      void Draw3DLine(vec3& pos1, vec3& pos2) const override;
      void FacadeDrawGraphEdges(ManWayPoint* manWayPoints) override;
      void FacadeDrawBoundingBox(Entity* entity, bool colliding) override;
      void FacadeDrawBoundingPlane(Entity* entity) const override;

      //scene::ISceneManager* GetSceneManager() { return smgr; };
      //scene::ICameraSceneNode* GetCamera1() { return camera1; };
      static bool showDebug;


   private:
};
