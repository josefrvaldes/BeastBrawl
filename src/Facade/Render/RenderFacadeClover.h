#pragma once

#include "RenderFacade.h"
#include <EventManager/Event.h>
#include <EventManager/EventManager.h>

#include "../../../CLEngine/src/CLEngine.h"

#include <codecvt>
#include <iostream>
#include <locale>
#include <memory>
#include <string>
#include <unordered_map>


using namespace std;
using namespace CLE;
class RenderFacadeClover : public RenderFacade {
   public:
      RenderFacadeClover();
      ~RenderFacadeClover() override;
      const uint16_t FacadeAddObject(Entity*) override;
      void FacadeAddSphereOnObject(Entity* entity) override;
      const uint16_t FacadeAddObjectCar(Entity*) override;
      const uint16_t FacadeAddObjectTotem(Entity* entity) override;
      const void FacadeAddObjects(vector<Entity*>) override;
      void FacadeAddCamera(Entity*) override;
      void UpdateCamera(Entity*, ManCar* manCars) override;
      bool FacadeRun() override;
      uint32_t FacadeGetTime() const override;
      vector<Constants::InputTypes> FacadeCheckInputMulti() override;
      void FacadeCheckInputSingle() override;
      int FacadeGetFPS() const override;
      void FacadeSetWindowCaption(std::string, int) const override;
      void FacadeBeginScene() const override;
      void FacadeDrawAll() const override;
      void FacadeEndScene() const override;
      void FacadeDeviceDrop() override;
      void FacadeDraw() const override;
      void DeleteEntity(Entity*) override;
      void FacadeDrawMenu() override;
      void FacadeDrawPause() override;
      void FacadeDrawEndRace() override;
      void FacadeDrawLobbyMulti() override;
      void FacadeDrawLobbyMultiExit() override;
      void FacadeDrawControler() override;
      void FacadeInitMenu() override;
      void FacadeInitPause() override;
      void FacadeInitEndRace() override;
      void FacadeInitLobbyMulti() override;
      void FacadeInitControler() override;
      void FacadeInitHUD() override;
      void FacadeCheckInputMenu() override;
      void FacadeCheckInputPause() override;
      void FacadeCheckInputEndRace() override;
      void FacadeCheckInputLobbyMulti() override;
      void FacadeCheckInputControler() override;
      void FacadeUpdatePowerUpHUD(DataMap* d) override;
      void FacadeDrawHUD(Entity* car, ManCar* manCars) override;
      void FacadeSuscribeEvents() override;
      void FacadeAddPlates(Manager* manNamePlates) override;
      void FacadeUpdatePlates(Manager* manNamePlates) override;
      void ThrowEventChangeToMulti(uint16_t IdOnline, const vector<uint16_t> IdPlayersOnline) override;

      //DEBUG
      void Draw3DLine(vec3& pos1, vec3& pos2, uint16_t r, uint16_t g, uint16_t b) const override;
      void Draw3DLine(vec3& pos1, vec3& pos2) const override;
      void FacadeDrawGraphEdges(ManWayPoint* manWayPoints) const override;
      void FacadeDrawBoundingBox(Entity* entity, bool colliding) const override;
      void FacadeDrawBoundingPlane(Entity* entity) const override;
      void FacadeDrawBoundingGround(Entity* entity) const override;
      void FacadeDrawBoundingOBB(Entity* entity) const override;
      void FacadeDrawAIDebug(ManCar* manCars, ManNavMesh* manNavMesh, ManWayPoint* manWayPoint) const override;
      void FacadeDrawAIDebugPath(Entity* carAI, ManWayPoint* manWayPoint) const override;

      void SetShowDebug(bool b) override { showDebug = b;};
      void SetShowDebugAI(bool b) override { showAIDebug = b;};
      void SetIDCarAIToDebug(int id) override {idCarAIToDebug = id;};

      bool GetShowDebug() override { return showDebug;};
      bool GetShowDebugAI() override { return showAIDebug;};
      int  GetIDCarAIToDebug() override { return idCarAIToDebug;};

      //Metodos exclusivos de RenderClover
      CLEngine* GetDevice() { return device;};

      inline static bool showDebug = false;
      inline static bool showAIDebug = false;



   private:

      CLEngine* device;
      CLNode* smgr;
      CLResourceManager* resourceManager;
      CLNode* camera1;
};
