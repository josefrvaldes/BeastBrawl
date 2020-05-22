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

#include <cstdint>
#include <iostream>
#include <string>
#include <chrono>
#include <Entities/Entity.h>
#include <EventManager/EventManager.h>
#include <Managers/ManWayPoint.h>
#include <Managers/ManCar.h>
#include <Managers/ManGameRules.h>
#include <Managers/ManHUDEvent.h>
#include <Components/CPowerUp.h>
#include <Components/CCar.h>
#include <Managers/Manager.h>
#include "../../Constants.h"


using namespace chrono;


class RenderFacade {
   public:


    //RenderFacade() = default;
    virtual ~RenderFacade() = default;
    virtual const uint16_t FacadeAddObject(Entity*) = 0;
    virtual const uint16_t FacadeAddObject(Entity*, bool mainCar_) = 0;
    virtual const uint16_t FacadeAddStaticObject(Entity*) = 0;
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
    virtual void DeleteEntity(Entity*) = 0;
    virtual void FacadeSetVisibleEntity(Entity*,bool) = 0;
    virtual void FacadeSetWindowSize(DataMap* d) = 0;

    virtual void FacadeDraw() const = 0;
    virtual void FacadeDrawIntro() = 0;
    virtual void FacadeDrawMenu() = 0;
    virtual void FacadeDrawSelectCharacter() = 0;
    virtual void FacadeDrawPause() = 0;
    virtual void FacadeDrawEndRace() = 0;
    virtual void FacadeDrawEndTournament() = 0;
    virtual void FacadeDrawLobbyMultiConnecting() = 0;
    virtual void FacadeDrawLobbyMultiExit() = 0;
    virtual void FacadeDrawLobbyMultiSelChar() = 0;
    virtual void FacadeDrawLobbyMultiWait() = 0;
    virtual void FacadeDrawControler() = 0;
    virtual void FacadeDrawCredits() = 0;
    virtual void FacadeDrawGameOptions() = 0;
    virtual void FacadeDrawTournamentOptions() = 0;
    virtual void FacadeDrawSettings() = 0;

    virtual void FacadeInitIntro() = 0;
    virtual void FacadeInitMenu() = 0;
    virtual void FacadeInitSelectCharacter() = 0;
    virtual void FacadeInitPause() = 0;
    virtual void FacadeInitEndRace() = 0;
    virtual void FacadeInitEndTournament() = 0;
    virtual void FacadeInitLobbyMulti() = 0;
    virtual void FacadeInitControler() = 0;
    virtual void FacadeInitHUD() = 0;
    virtual void FacadeInitCredits() = 0;
    virtual void FacadeInitGameOptions() = 0;
    virtual void FacadeInitTournamentOptions() = 0;
    virtual void FacadeInitSettings() = 0;

    virtual void FacadeCheckInputIntro() = 0;
    virtual void FacadeCheckInputMenu() = 0;
    virtual void FacadeCheckInputSelectCharacter() = 0;
    virtual void FacadeCheckInputPause() = 0;
    virtual void FacadeCheckInputEndRace() = 0;
    virtual void FacadeCheckInputEndTournament() = 0;
    virtual void FacadeCheckInputLobbyMultiConnecting() = 0;
    virtual void FacadeCheckInputLobbyMultiExit() = 0;
    virtual void FacadeCheckInputLobbyMultiSelChar() = 0;
    virtual void FacadeCheckInputLobbyMultiWait() = 0;
    virtual void FacadeCheckInputControler() = 0;
    virtual void FacadeCheckInputCredits() = 0;
    virtual void FacadeCheckInputGameOptions() = 0;
    virtual void FacadeCheckInputTournamentOptions() = 0;
    virtual void FacadeCheckInputSettings() = 0;
    virtual bool FacadeCheckShowHUD() = 0;

    virtual void FacadeUpdatePowerUpHUD(DataMap* d) = 0;
    virtual void FacadeInitResources(mainCharacter character) = 0;
    virtual void FacadeDrawHUD(Entity* car, ManCar* manCars, Entity* globalClock, ManHUDEvent*, ManGameRules*, bool ingame) = 0;
    virtual void FacadeSuscribeEvents() = 0;
    virtual void FacadeSuscribeEventsSettings() = 0;
    virtual void FacadeAddPlates(Manager* manNamePlates) = 0;
    virtual void FacadeUpdatePlates(Manager* manNamePlates, Manager* manCars) = 0;
    virtual void FacadeUpdateMeshesLoD(vector<shared_ptr<Entity>> entities) = 0;
    virtual void FacadeUpdateAnimationsLoD(vector<shared_ptr<Entity>> entities) = 0;
    virtual void FacadeAnimate(vector<shared_ptr<Entity>> entities) = 0;
    virtual void ThrowEventChangeToMulti(uint16_t IdOnline, const vector<uint16_t> IdPlayersOnline) = 0;
    virtual void FacadeAddSkybox(string right,string left,string top,string bottom,string front,string back) = 0;
    virtual void FacadeAddShadowMapping(unsigned int lightId) = 0;
    virtual void FacadeAddGrass(float _width, float _height, const glm::vec3& _position, const glm::vec3& _scale, bool realistGrass) = 0;
    virtual void FacadeAddGrass(float radious, const glm::vec3& _position, const glm::vec3& _scale, bool realistGrass) = 0;
    virtual void CleanScene() = 0;
    virtual void FacadeUpdateViewport() = 0;

    virtual void FacadeInitParticleSystem(DataMap* d) const = 0;
    virtual void FacadeSetParticlesVisibility(DataMap* d) const = 0;
    virtual void FacadeSetGrassActivate(DataMap* d) const = 0;
    virtual void FacadeSetShadowsActivate(DataMap* d) const = 0;
    virtual void FacadeUpdateVisibility(DataMap* d) = 0;
    virtual bool FacadeOctreeInCamera(float size, const glm::vec3& pos) = 0;
    virtual void FacadeSetOctreeVisibleById(unsigned int id, bool v) = 0;



    virtual void FacadeReleaseSelectCharacter() = 0;
    virtual void FacadeReleaseOptions() = 0;
    virtual void FacadeReleaseSettings() = 0;
    virtual void FacadeReleaseStateInGame() = 0;
    virtual void FacadeReleaseMenu() = 0;
    virtual void FacadeReleaseTournament() = 0;

    virtual float FacadeGetFovActualCamera() = 0;
    virtual glm::vec3 FacadeGetTargetActualCamera() = 0;
    virtual glm::vec3 FacadeGetPositionActualCamera() = 0;
    //DEBUG
    virtual void Draw3DLine(vec3 &pos1, vec3 &pos2, uint16_t r, uint16_t g, uint16_t b) const = 0;
    virtual void Draw3DLine(vec3 &pos1, vec3 &pos2) const = 0;
    virtual void Draw2DImage(float x_, float y_, int width_, int height_, float depth_, string file_, bool) const = 0;
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

    virtual void SetCamTarget(glm::vec3 pos) = 0;

    virtual bool GetShowDebug() = 0;
    virtual bool GetShowDebugAI() = 0;
    virtual int  GetIDCarAIToDebug() = 0;
    virtual std::tuple<int, int> GetScreenSize() = 0;

    virtual void ResetInputGameOptions() = 0;
    virtual void ResetInputTournamentOptions() = 0;
    virtual void ResetInputCharacter() = 0;
    virtual void SetMenuEndRace(bool) = 0;
    virtual bool GetMenuEndRace() = 0;
    virtual void SetMenuEndTournament(uint8_t) = 0;
    virtual uint8_t GetMenuEndTournament() = 0;

    int GetNumEnemyCars() { return numEnemyCars; };
    void SetNumEnemyCars(int n) { numEnemyCars = n; };

   protected:
    uint16_t idCar { 0 };
    uint16_t idTotem { 0 };
    uint16_t idCarWithTotem { 0 };
    int numEnemyCars { 0 };
    int currentPowerUp { 0 };


    time_point<system_clock> timeStart;
    const int64_t inputDelay { 500 };       // 0.5 segundos
    const int64_t inputDelayCamera { 250 };       // 0.5 segundos
    bool invertedCam { false };
    bool totemCamActive { false };
    int idCarAIToDebug { 0 };
};
