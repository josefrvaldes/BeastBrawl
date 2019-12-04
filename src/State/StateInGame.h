#ifndef STATEINGAME_H
#define STATEINGAME_H


#include "../Game.h"
#include "../Entities/Car.h"
#include "../Entities/GameObject.h"
#include "../Entities/Camera.h"
#include "../Entities/PowerUp.h"
#include "../Entities/WayPoint.h"
#include "../Entities/CarAI.h"
#include "../Systems/Physics.h"
#include "../Managers/ManPowerUp.h"
#include "../Managers/ManWayPoint.h"
#include "../Managers/ManCarAI.h"
#include "../EventManager/EventManager.h"
#include "../Facade/Render/RenderFacadeManager.h"
#include "../Facade/Input/InputFacadeManager.h"
#include "../Facade/Physics/PhysicsFacadeManager.h"
#include "../Systems/Physics.h"
#include "../Systems/PhysicsAI.h"
#include "../fuzzyLogic/fuzzyLogic.h"
#include "../behaviourTree/behaviourTree.h"
#include "../../lib/glm/vec3.hpp"

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
        CarAI* carAI;
        GameObject* ground;
        Camera* cam;
        RenderFacadeManager* renderFacadeManager;
        InputFacadeManager* inputFacadeManager;
        PhysicsFacadeManager* physicsFacadeManager;
        RenderFacade* renderEngine;
        InputFacade* inputEngine;
        PhysicsFacade* physicsEngine;
        ManPowerUp *manPowerUps;
        ManWayPoint *manWayPoint;
        PhysicsAI* physicsAI;

        int lastFPS = -1;
        uint32_t then;
	
};


#endif  // STATEINGAME_H 