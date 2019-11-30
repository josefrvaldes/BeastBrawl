#include "StateInGame.h"
#include <iostream>

using namespace std;

StateInGame::StateInGame(){
    // constructor
    std::cout << "Estado InGame Creado" << std::endl;
    
    eventManager = EventManager::GetInstance();

    car = new GameObject(10.0,20.0,30.0,    0.0,0.0,0.0,    1.0,1.0,1.0, "particle.bmp", "ninja.b3d",20,0.15,0.1,0.25);
    ground = new GameObject(10.0,10.0,30.0,    0.0,0.0,0.0,    100.0,1.0,100.0, "wall.jpg", "ninja.b3d",20,0.15,0.1,0.25);
    cam = new Camera(10.0,20.0,30.0,    0.0,0.0,0.0,    1.0,1.0,1.0);

	renderFacadeManager = RenderFacadeManager::GetInstance();
	renderFacadeManager->InitializeIrrlicht();

    inputFacadeManager = InputFacadeManager::GetInstance();
    inputFacadeManager->InitializeIrrlicht();

    physicsFacadeManager = PhysicsFacadeManager::GetInstance();
    physicsFacadeManager->InitializeIrrlicht();
    
    //Almacenamos los motores
	renderEngine   = renderFacadeManager->GetRenderFacade();
    inputEngine    = inputFacadeManager->GetInputFacade();
    physicsEngine  = physicsFacadeManager->GetPhysicsFacade();

    renderEngine->FacadeAddObject(car);
    renderEngine->FacadeAddObject(ground);
    
    renderEngine->FacadeAddCamera(cam);

    lastFPS = -1;
    then = renderEngine->FacadeGetTime();
}



StateInGame::~StateInGame(){
    // destructor
}



void StateInGame::Render(){

}


void StateInGame::Update(){
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
}