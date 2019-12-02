#ifndef STATEINGAME_H
#define STATEINGAME_H


#include "../Game.h"
#include "../Entities/Hero.h"
#include "../Entities/Car.h"
#include "../Entities/GameObject.h"
#include "../Entities/Camera.h"
#include "../Entities/PowerUp.h"
#include "../Systems/Physics.h"
#include "../Managers/ManPowerUp.h"
#include "../EventManager/EventManager.h"
#include "../Facade/Render/RenderFacadeManager.h"
#include "../Facade/Input/InputFacadeManager.h"
#include "../Facade/Physics/PhysicsFacadeManager.h"
#include "../Systems/Physics.h"
#include "../fuzzyLogic/fuzzyLogic.h"

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
        Car* car;
        GameObject* ground;
        Camera* cam;
        RenderFacadeManager* renderFacadeManager;
        InputFacadeManager* inputFacadeManager;
        PhysicsFacadeManager* physicsFacadeManager;
        RenderFacade* renderEngine;
        InputFacade* inputEngine;
        PhysicsFacade* physicsEngine;
        ManPowerUp *manPowerUps;

        int lastFPS = -1;
        uint32_t then;
	
};


#endif  // STATEINGAME_H 