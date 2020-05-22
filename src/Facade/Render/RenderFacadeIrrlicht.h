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

#include <irrlicht/irrlicht.h>
#include <EventManager/Event.h>
#include <EventManager/EventManager.h>
#include "RenderFacade.h"
#include "../Input/InputFacadeIrrlicht.h"


#include <codecvt>
#include <iostream>
#include <locale>
#include <memory>
#include <string>
#include <unordered_map>



using namespace std;
using namespace irr;
class RenderFacadeIrrlicht : public RenderFacade {
   public:


    RenderFacadeIrrlicht();
    ~RenderFacadeIrrlicht() override;
    const uint16_t FacadeAddObject(Entity*) override;
    const uint16_t FacadeAddObject(Entity*, bool mainCar_) override;
    const uint16_t FacadeAddStaticObject(Entity*) override { return 1; };
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
    void FacadeSetVisibleEntity(Entity*,bool) override {};
    void FacadeSetWindowSize(DataMap* d) override {};


    void FacadeDrawIntro() override;
    void FacadeDrawMenu() override;
    void FacadeDrawSelectCharacter() override;
    void FacadeDrawGameOptions() override;
    void FacadeDrawTournamentOptions() override {};
    void FacadeInitResources(mainCharacter character) override;
    void FacadeDrawPause() override;
    void FacadeDrawEndRace() override;
    void FacadeDrawEndTournament() override {};
    void FacadeDrawLobbyMultiConnecting() override;
    void FacadeDrawLobbyMultiExit() override;
    void FacadeDrawLobbyMultiSelChar() override {};
    void FacadeDrawLobbyMultiWait() override {};

    void FacadeDrawControler() override;
    void FacadeDrawCredits() override;
    void FacadeDrawSettings() override;

    void FacadeInitIntro() override;
    void FacadeInitMenu() override;
    void FacadeInitSelectCharacter() override;
    void FacadeInitGameOptions() override;
    void FacadeInitTournamentOptions() override {};
    void FacadeInitPause() override;
    void FacadeInitEndRace() override;
    void FacadeInitEndTournament() override {};
    void FacadeInitLobbyMulti() override;
    void FacadeInitControler() override;
    void FacadeInitHUD() override;
    void FacadeInitCredits() override;
    void FacadeInitSettings() override;

    void FacadeCheckInputIntro() override;
    void FacadeCheckInputMenu() override;
    void FacadeCheckInputSelectCharacter() override;
    void FacadeCheckInputGameOptions() override;
    void FacadeCheckInputTournamentOptions() override {};
    void FacadeCheckInputPause() override;
    void FacadeCheckInputEndRace() override;
    void FacadeCheckInputEndTournament() override {};
    void FacadeCheckInputLobbyMultiConnecting() override;
    void FacadeCheckInputLobbyMultiExit() override {};
    void FacadeCheckInputLobbyMultiSelChar() override {};
    void FacadeCheckInputLobbyMultiWait() override {};
    void FacadeCheckInputControler() override;
    void FacadeCheckInputCredits() override;
    void FacadeCheckInputSettings() override;
    bool FacadeCheckShowHUD() override { return true; };

    void FacadeUpdatePowerUpHUD(DataMap* d) override;
    void FacadeDrawHUD(Entity* car, ManCar* manCars, Entity* globalClock, ManHUDEvent* manHud, ManGameRules* manGR, bool ingame) override;
    void FacadeSuscribeEvents() override;
    void FacadeSuscribeEventsSettings() override;
    void FacadeAddPlates(Manager* manNamePlates) override;
    void FacadeUpdatePlates(Manager* manNamePlates, Manager* manCars) override;
    void FacadeUpdateMeshesLoD(vector<shared_ptr<Entity>> entities) override;
    void FacadeAnimate(vector<shared_ptr<Entity>> entities) override;
    void FacadeUpdateAnimationsLoD(vector<shared_ptr<Entity>> entities) override;
    void ThrowEventChangeToMulti(uint16_t IdOnline, const vector<uint16_t> IdPlayersOnline) override;
    void FacadeAddSkybox(string right,string left,string top,string bottom,string front,string back) override;
    void FacadeAddShadowMapping(unsigned int lightId) override;
    void FacadeAddGrass(float _width, float _height, const glm::vec3& _position, const glm::vec3& _scale, bool realistGrass) override {};
    void FacadeAddGrass(float radious, const glm::vec3& _position, const glm::vec3& _scale, bool realistGrass) override {};
    void CleanScene() override;
    void FacadeUpdateViewport() override;
    
    void FacadeInitParticleSystem(DataMap* d) const override {};
    void FacadeSetParticlesVisibility(DataMap* d) const {};
    void FacadeSetGrassActivate(DataMap* d) const {};
    void FacadeSetShadowsActivate(DataMap* d) const {};
    void FacadeUpdateVisibility(DataMap* d) override {};
    bool FacadeOctreeInCamera(float size, const glm::vec3& pos) override {return true;};
    void FacadeSetOctreeVisibleById(unsigned int id, bool v) override {};


    void FacadeReleaseSelectCharacter() override {};
    void FacadeReleaseOptions() override {};
    void FacadeReleaseSettings() override {};
    void FacadeReleaseStateInGame() override {};
    void FacadeReleaseMenu() override {};
    void FacadeReleaseTournament() override {};


    float FacadeGetFovActualCamera() override {return 0;};
    glm::vec3 FacadeGetTargetActualCamera() override {return glm::vec3(0.0);};
    glm::vec3 FacadeGetPositionActualCamera() override {return glm::vec3(0.0);};

    //DEBUG
    void Draw3DLine(vec3& pos1, vec3& pos2, uint16_t r, uint16_t g, uint16_t b) const override;
    void Draw3DLine(vec3& pos1, vec3& pos2) const override;
    void Draw2DImage(float x_, float y_, int width_, int height_, float depth_, string file_, bool) const override;
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
    void SetCamTarget(glm::vec3 pos) override;

    bool GetShowDebug() override { return showDebug;};
    bool GetShowDebugAI() override { return showAIDebug;};
    int  GetIDCarAIToDebug() override { return idCarAIToDebug;};
    std::tuple<int, int> GetScreenSize() override;

    void ResetInputGameOptions() override;
    void ResetInputTournamentOptions() override {};
    void ResetInputCharacter() override;

    void SetMenuEndRace(bool b) override { menuER = b; };
    bool GetMenuEndRace() override { return menuER; };
    void SetMenuEndTournament(uint8_t b) override { menuET = b; };
    uint8_t GetMenuEndTournament() override { return menuET; };


    scene::ISceneManager* GetSceneManager() { return smgr; };
    scene::ICameraSceneNode* GetCamera1() { return camera1; };
    inline static bool showDebug = false;
    inline static bool showAIDebug = false;

   private:
    IrrlichtDevice* device;
    video::IVideoDriver* driver;
    scene::ISceneManager* smgr;
    scene::ICameraSceneNode* camera1;
    MyEventReceiver receiver;
    video::ITexture* introBG;
    video::ITexture* menuBG;
    video::ITexture* controlerBG;
    video::ITexture* pauseBG;
    video::ITexture* endRaceBG;
    video::ITexture* lobbyMultBG;
    video::ITexture* lobbyMultFullBG;
    video::ITexture* whiteBG;
    video::ITexture* powerUps[7];
    gui::IGUIFont* font;

    bool menuER { false };
    uint8_t menuET { 0 };


    enum InputXBox{BUTTON_A, BUTTON_B, BUTTON_X, BUTTON_Y, BUTTON_LB, BUTTON_RB, BUTTON_BACK, BUTTON_START, BUTTON_XBOX, BUTTON_STICK_L, BUTTON_STICK_R, END};
    std::unordered_map<InputXBox, bool> inputsPressed;
    core::array<SJoystickInfo> joystickInfo;
    bool IsInputPressed(InputXBox input);
    void SetValueInput(InputXBox input, bool valuePressed);
    //unordered_map<uint16_t,scene::ISceneNode*> sceneObjects; // CId - ISceneNode*
};
