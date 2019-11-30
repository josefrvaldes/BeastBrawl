#ifndef STATEINGAME_H
#define STATEINGAME_H


#include "../Game.h"
#include "../Entities/Hero.h"
#include "../Entities/GameObject.h"
#include "../Entities/Camera.h"
#include "../Systems/Physics.h"
#include "../EventManager/EventManager.h"
#include "../Facade/Render/RenderFacadeManager.h"
#include "../Facade/Input/InputFacadeManager.h"
#include "../Facade/Physics/PhysicsFacadeManager.h"

#include <iostream>
#include <list>
#include <string>
#include <cstdint>
#include <memory>

class StateInGame : public State{
    public:
        StateInGame();
        virtual ~StateInGame();
        void Update();
        void Render();
        virtual States GetState() { return State::States::INGAME; };


    private:
        EventManager* eventManager;
        GameObject* car;
        GameObject* ground;
        Camera* cam;
        RenderFacadeManager* renderFacadeManager;
        InputFacadeManager* inputFacadeManager;
        PhysicsFacadeManager* physicsFacadeManager;
        RenderFacade* renderEngine;
        InputFacade* inputEngine;
        PhysicsFacade* physicsEngine;

        int lastFPS = -1;
        uint32_t then;
	
};


#endif  // STATEINGAME_H 