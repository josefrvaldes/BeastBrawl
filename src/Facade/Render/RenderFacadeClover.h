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
#include <vector>
#include <unordered_map>


using namespace CLE;

class RenderFacadeClover : public RenderFacade {
   public:
      RenderFacadeClover();
      ~RenderFacadeClover() override;
      const uint16_t FacadeAddObject(Entity*) override;
      void FacadeAddSphereOnObject(Entity* entity) override;
      const uint16_t FacadeAddObjectCar(Entity*) override;
      const uint16_t FacadeAddObjectTotem(Entity* entity) override;
      const void FacadeAddObjects(std::vector<Entity*>) override;
      void FacadeAddCamera(Entity*) override;
      void UpdateCamera(Entity*, ManCar* manCars) override;
      bool FacadeRun() override;
      uint32_t FacadeGetTime() const override;
      std::vector<Constants::InputTypes> FacadeCheckInputMulti() override;
      void FacadeCheckInputSingle() override;
      int FacadeGetFPS() const override;
      void FacadeSetWindowCaption(std::string, int) const override;
      void FacadeBeginScene() const override;
      void FacadeDrawAll() const override;
      void FacadeEndScene() const override;
      void FacadeDeviceDrop() override;
      void DeleteEntity(Entity*) override;
      void FacadeSetVisibleEntity(Entity*,bool) override;


      void FacadeDraw() const override;
      void FacadeDrawIntro() override;
      void FacadeDrawMenu() override;
      void FacadeDrawSelectCharacter() override;
      void FacadeDrawGameOptions() override;
      void FacadeInitResources() override;
      void FacadeDrawPause() override;
      void FacadeDrawEndRace() override;
      void FacadeDrawLobbyMulti() override;
      void FacadeDrawLobbyMultiExit() override;
      void FacadeDrawControler() override;
      void FacadeDrawCredits() override;
      void FacadeDrawSettings() override;

      void FacadeInitIntro() override;
      void FacadeInitMenu() override;
      void FacadeInitSelectCharacter() override;
      void FacadeInitGameOptions() override;
      void FacadeInitPause() override;
      void FacadeInitEndRace() override;
      void FacadeInitLobbyMulti() override;
      void FacadeInitControler() override;
      void FacadeInitHUD() override;
      void FacadeInitCredits() override;
      void FacadeInitSettings() override;

      void FacadeCheckInputIntro() override;
      void FacadeCheckInputMenu() override;
      void FacadeCheckInputSelectCharacter() override;
      void FacadeCheckInputGameOptions() override;
      void FacadeCheckInputPause() override;
      void FacadeCheckInputEndRace() override;
      void FacadeCheckInputLobbyMulti() override;
      void FacadeCheckInputControler() override;
      void FacadeCheckInputCredits() override;
      void FacadeCheckInputSettings() override;

      void FacadeUpdatePowerUpHUD(DataMap* d) override;
      void FacadeDrawHUD(Entity* car, ManCar* manCars) override;
      void FacadeSuscribeEvents() override;
      void FacadeAddPlates(Manager* manNamePlates) override;
      void FacadeUpdatePlates(Manager* manNamePlates) override;
      void FacadeUpdateMeshesLoD(vector<shared_ptr<Entity>> entities) override;
      void ThrowEventChangeToMulti(uint16_t IdOnline, const std::vector<uint16_t> IdPlayersOnline) override;
      void FacadeAddSkybox(string right,string left,string top,string bottom,string front,string back) override;
      void FacadeAddShadowMapping(unsigned int lightId) override;
      void CleanScene() override;
      void FacadeUpdateViewport() override;

      void FacadeInitParticleSystem(DataMap* d) override;

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

      void SetMenuEndRace(bool b) override { menuER = b; };
      bool GetMenuEndRace() override { return menuER; };

      void ResetInputGameOptions() override;
      void ResetInputCharacter() override;

      //Metodos exclusivos de RenderClover
      CLEngine* GetDevice() { return device;};

      inline static bool showDebug = false;
      inline static bool showAIDebug = false;



    private:

        std::string powerUps[7];

        //Menu
        int inputMenu { 0 };
        int maxInputMenu { 5 };
        //Pause
        int inputPause { 0 };
        int maxInputPause { 1 };
        //Seleccion de personaje
        int inputSC { 0 };
        int maxInputSC { 5 };
        //End Race
        bool menuER { false };
        int inputER { 0 };
        int maxInputER { 2 };
        //Opciones de partida (Hay que contar el 0)
        int option { 2 };
        std::vector<int> inputGO {1,1,0};
        int maxInputGO[3] {3, 2, 0};
        //Ajustes
        int optionSettings { 3 };
        std::vector<int> inputSettings {1,0,0,1};
        int maxInputSettings[4] {3, 1, 2, 0};

        CLEngine* device {nullptr};
        CLNode* smgr {nullptr};
        CLResourceManager* resourceManager {nullptr};
        CLNode* camera1 {nullptr};
};
