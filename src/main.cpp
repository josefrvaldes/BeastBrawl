
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
//#include "Facade/InputFacadeManager.h"

#include "Game.h"
#include <iostream>
#include <list>
#include <string>
#include <cstdint>
#include <memory>




void pruebaEvent1(Data d){
    
    std::cout << "Soy pruebaEvent1"<< "\n";
}
void pruebaEvent2(Data d){
    std::cout << "Soy pruebaEvent2"<< "\n";
}


int main()
{
    Game *game = Game::GetInstance();
    
    game->SetState(State::States::INGAME);
    game->InitGame();


    EventManager eventManager = EventManager::GetInstance();
    eventManager.Suscribe(Listener {EventType::PRIORIDAD1,pruebaEvent1, "suscriptor1"});
    eventManager.Suscribe(Listener {EventType::PRIORIDAD1,pruebaEvent1, "suscriptor2"});
    eventManager.Suscribe(Listener {EventType::PRIORIDAD2,pruebaEvent2, "suscriptor3"});

    Data d;

    eventManager.AddEvent(Event {EventType::PRIORIDAD1,d});
    eventManager.AddEvent(Event {EventType::PRIORIDAD2,d});
    eventManager.AddEvent(Event {EventType::PRIORIDAD1,d});
    
    eventManager.Update();  
    cout << "------------------------------\n";
    eventManager.UnSuscribe(EventType::PRIORIDAD1,"suscriptor2");

    eventManager.AddEvent(Event {EventType::PRIORIDAD1,d});
    eventManager.AddEvent(Event {EventType::PRIORIDAD2,d});
    eventManager.AddEvent(Event {EventType::PRIORIDAD1,d});

    
    eventManager.Update();  

    GameObject *go = new GameObject();

    Component* cId   = new CId();
    Component* cType = new CType(ModelType::Sphere);
    Component* cTransformable = new CTransformable(10.0,20.0,30.0,    0.0,0.0,0.0,    1.0,1.0,1.0);
    Component* cTexture = new CTexture(std::string("wall.bmp"));
    Component* cMesh   = new CMesh(std::string("media/ninja.b3d"));
    go->AddComponent(cId);
    go->AddComponent(cType);
    go->AddComponent(cTransformable);
    go->AddComponent(cTexture);
    go->AddComponent(cMesh);

	RenderFacadeManager renderFacadeManager = RenderFacadeManager::GetInstance();
	renderFacadeManager.InitializeIrrlicht();

    //InputFacadeManager* inputFacadeManager = new InputFacadeManager();
    //inputFacadeManager->InitializeIrrlicht();
    
    //auto inputEngine  = inputFacadeManager->GetInputFacade();
	auto renderEngine = renderFacadeManager.GetRenderFacade();
	renderEngine->FacadeAddObject(go);
    
    renderEngine->FacadeAddCamera();

    int lastFPS = -1;
    uint32_t then = renderEngine->FacadeGetTime();

    while(renderEngine->FacadeRun()){
        const uint32_t now = renderEngine->FacadeGetTime();
        
        const float frameDeltaTime = (float)(now - then) / 1000.0;
        then = now;
        //inputEngine->CheckInputs();
        renderEngine->FacadeCheckInput(frameDeltaTime);

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