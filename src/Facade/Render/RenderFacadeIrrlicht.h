#pragma once

#include "RenderFacade.h"
#include "../../../lib/irrlicht/irrlicht.h"
#include "../../EventManager/EventManager.h"
#include "../../EventManager/Event.h"
#include "../Input/InputFacadeIrrlicht.h"
#include "../Input/InputFacadeManager.h"

//#include "../../State/State.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include <locale>
#include <codecvt>

using namespace std;
using namespace irr;


class RenderFacadeIrrlicht : public RenderFacade {
    public:
        RenderFacadeIrrlicht();
        ~RenderFacadeIrrlicht() override;
        const uint16_t FacadeAddObject(Entity*) override;
        const uint16_t FacadeAddObjectCar(Entity*) override;
		const void FacadeAddObjects(vector<Entity*>) override;
		void FacadeAddCamera(Entity*) override;
        void UpdateTransformable(Entity*) override;
        void UpdateCamera(Entity*) override;
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
		void FacadeInitMenu() override;
		void FacadeInitPause() override;
		void FacadeInitHUD() override;
		void FacadeCheckInputMenu() override;
		void FacadeCheckInputPause() override;
		void FacadeUpdatePowerUpHUD(DataMap d) override;
		void FacadeDrawHUD(Entity* car) override;
		void FacadeSuscribeEvents() override;

		//DEBUG
        void FacadeDrawGraphEdges(ManWayPoint* manWayPoints) override;
		void FacadeDrawBoundingBox(Entity* entity, bool colliding) override;


		scene::ISceneManager* GetSceneManager() {return smgr;};
		scene::ICameraSceneNode* GetCamera1() {return camera1;};


    private:
        IrrlichtDevice* device;
        video::IVideoDriver* driver;
        scene::ISceneManager* smgr;
		scene::ICameraSceneNode* camera1;
		MyEventReceiver receiver;
		video::ITexture* menuBG;
		video::ITexture* pauseBG;
		video::ITexture* powerUps[7];
		gui::IGUIFont* font;
		bool showDebug = false;
		int currentPowerUp = 0;

        //unordered_map<uint16_t,scene::ISceneNode*> sceneObjects; // CId - ISceneNode*
};

