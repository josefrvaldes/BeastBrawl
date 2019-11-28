#include "Entities/Hero.h"
#include "Entities/GameObject.h"
#include "Entities/Camera.h"
#include "Systems/Physics.h"
#include "EventManager/EventManager.h"
#include "Facade/Render/RenderFacadeManager.h"
#include "Facade/Input/InputFacadeManager.h"
#include "Facade/Physics/PhysicsFacadeManager.h"

#include "Game.h"
#include <iostream>
#include <list>
#include <string>
#include <cstdint>
#include <memory>



int main()
{
    Game *game = Game::GetInstance();
    
    game->SetState(State::States::INGAME);
    game->InitGame();

    EventManager* eventManager = EventManager::GetInstance();

    GameObject *car = new GameObject(10.0,20.0,30.0,    0.0,0.0,0.0,    1.0,1.0,1.0, "particle.bmp", "ninja.b3d",20,0.15,0.1,0.25);
    GameObject *ground = new GameObject(10.0,10.0,30.0,    0.0,0.0,0.0,    100.0,1.0,100.0, "wall.jpg", "ninja.b3d",20,0.15,0.1,0.25);
    Camera *cam = new Camera(10.0,20.0,30.0,    0.0,0.0,0.0,    1.0,1.0,1.0);

	RenderFacadeManager* renderFacadeManager = RenderFacadeManager::GetInstance();
	renderFacadeManager->InitializeIrrlicht();

    InputFacadeManager* inputFacadeManager = InputFacadeManager::GetInstance();
    inputFacadeManager->InitializeIrrlicht();

    PhysicsFacadeManager* physicsFacadeManager = PhysicsFacadeManager::GetInstance();
    physicsFacadeManager->InitializeIrrlicht();
    
    //Almacenamos los motores
	auto renderEngine   = renderFacadeManager->GetRenderFacade();
    auto inputEngine    = inputFacadeManager->GetInputFacade();
    auto physicsEngine  = physicsFacadeManager->GetPhysicsFacade();

	const uint16_t carID    = renderEngine->FacadeAddObject(car);
    const uint16_t groundID = renderEngine->FacadeAddObject(ground);
    
    renderEngine->FacadeAddCamera(cam);

    int lastFPS = -1;
    uint32_t then = renderEngine->FacadeGetTime();

    while(renderEngine->FacadeRun()){
        eventManager->Update();  
        const uint32_t now = renderEngine->FacadeGetTime();
        
        const float frameDeltaTime = (float)(now - then) / 100.0;
        then = now;
        //inputEngine->CheckInputs(*car);
        renderEngine->FacadeCheckInput(frameDeltaTime,*car,*cam);
        renderEngine->UpdateTransformable(car);
        renderEngine->UpdateCamera(cam);

        renderEngine->FacadeDraw();


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
	
	return 0;
}