#include "Entities/Hero.h"
#include "Entities/GameObject.h"
#include "Components/CPosition.h"
#include "Components/CSpeed.h"
#include "Components/CId.h"
#include "Components/CType.h"
#include "Components/CTexture.h"
#include "Components/CMesh.h"
#include "Components/CTransformable.h"
#include "Systems/Physics.h"
#include "EventManager/EventManager.h"
#include "Facade/RenderFacadeManager.h"

#include "Game.h"
#include <iostream>
#include <list>
#include <cstdint>

#include <irrlicht.h>


using namespace irr;
//using namespace core;
//using namespace scene;
//using namespace video;
//using namespace io;
//using namespace gui;

void pruebaEvent1(Data d){
    
    std::cout << "Soy pruebaEvent1"<< "\n";
}

void pruebaEvent2(Data d){
    std::cout << "Soy pruebaEvent2"<< "\n";
}


int main()
{
    /*Game *game = Game::GetInstance();
    
    game->SetState(State::States::INGAME);
    game->InitGame();

    EventManager eventManager = EventManager::GetInstance();
    eventManager.Suscribe(Listener {EventType::PRIORIDAD1,pruebaEvent1});
    eventManager.Suscribe(Listener {EventType::PRIORIDAD2,pruebaEvent2});

    Data d;
    d.id = 15;

    eventManager.AddEvent(Event {EventType::PRIORIDAD3,d}); 
    eventManager.AddEvent(Event {EventType::PRIORIDAD1,d});
    eventManager.AddEvent(Event {EventType::PRIORIDAD2,d});
    eventManager.AddEvent(Event {EventType::PRIORIDAD1,d});

    
    eventManager.Update();  
*/
    GameObject *go = new GameObject();

    Component* cId   = new CId();
    Component* cType = new CType(ModelType::Sphere);
    Component* cTransformable = new CTransformable(10.0,20.0,30.0,    0.0,0.0,0.0,    1.0,1.0,1.0);
    Component* cTexture = new CTexture(std::string("media/wall.bmp"));
    Component* cMesh   = new CMesh(std::string("media/ninja.b3d"));
    go->AddComponent(cId);
    go->AddComponent(cType);
    go->AddComponent(cTransformable);
    go->AddComponent(cTexture);
    go->AddComponent(cMesh);


	RenderFacadeManager* renderFacadeManager = new RenderFacadeManager();
	renderFacadeManager->InitializeIrrlicht();

	auto renderEngine = renderFacadeManager->GetRenderFacade();
	renderEngine->FacadeAddObject(go);

    renderEngine->FacadeAddCamera();

    int lastFPS = -1;
    uint32_t then = renderEngine->FacadeGetTime();

    while(renderEngine->FacadeRun()){
        const uint32_t now = renderEngine->FacadeGetTime();
        
        const float frameDeltaTime = (float)(now - then) / 1000.0;
        then = now;
        renderEngine->FacadeCheckInput(frameDeltaTime);

        renderEngine->FacadeDraw();
        //renderEngine->FacadeBeginScene();
        //renderEngine->FacadeDrawAll();
//
        //renderEngine->FacadeEndScene();

        int fps = renderEngine->FacadeGetFPS();
		if (lastFPS != fps)
		{
			std::string tmp("Movement Example - Irrlicht Engine [");
			tmp += "] fps: ";
			tmp += fps;

			renderEngine->FacadeSetWindowCaption(tmp);
			lastFPS = fps;
		}

    }

    renderEngine->FacadeDeviceDrop();
//	/*
//	We have done everything, so lets draw it. We also write the current
//	frames per second and the name of the driver to the caption of the
//	window.
//	*/
//	int lastFPS = -1;
//
//	// In order to do framerate independent movement, we have to know
//	// how long it was since the last frame
//	u32 then = device->getTimer()->getTime();
//
//	// This is the movemen speed in units per second.
//	const f32 MOVEMENT_SPEED = 5.f;
//
//	while(device->run())
//	{
//		// Work out a frame delta time.
//		const u32 now = device->getTimer()->getTime();
//		const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
//		then = now;
//
//		/* Check if keys W, S, A or D are being held down, and move the
//		sphere node around respectively. */
//		core::vector3df nodePosition = node->getPosition();
//
//		if(receiver.IsKeyDown(irr::KEY_KEY_W))
//			nodePosition.Y += MOVEMENT_SPEED * frameDeltaTime;
//		else if(receiver.IsKeyDown(irr::KEY_KEY_S))
//			nodePosition.Y -= MOVEMENT_SPEED * frameDeltaTime;
//
//		if(receiver.IsKeyDown(irr::KEY_KEY_A))
//			nodePosition.X -= MOVEMENT_SPEED * frameDeltaTime;
//		else if(receiver.IsKeyDown(irr::KEY_KEY_D))
//			nodePosition.X += MOVEMENT_SPEED * frameDeltaTime;
//
//		node->setPosition(nodePosition);
//
//		driver->beginScene(true, true, video::SColor(255,113,113,133));
//
//		smgr->drawAll(); // draw the 3d scene
//		device->getGUIEnvironment()->drawAll(); // draw the gui environment (the logo)
//
//		driver->endScene();
//
//		int fps = driver->getFPS();
//
//		if (lastFPS != fps)
//		{
//			core::stringw tmp(L"Movement Example - Irrlicht Engine [");
//			tmp += driver->getName();
//			tmp += L"] fps: ";
//			tmp += fps;
//
//			device->setWindowCaption(tmp.c_str());
//			lastFPS = fps;
//		}
//	}
//
//	/*
//	In the end, delete the Irrlicht device.
//	*/
//	device->drop();
	
	return 0;
}