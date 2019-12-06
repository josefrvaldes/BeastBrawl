#ifndef STATEINGAME_H
#define STATEINGAME_H


#include "../Game.h"
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
#include "../behaviourTree/behaviourTree.h"
#include "../behaviourTree/selector.h"
#include "../behaviourTree/decorator.h"
#include "../behaviourTree/sequence.h"
#include "../../lib/glm/vec3.hpp"

#include <iostream>
#include <list>
#include <string>
#include <cstdint>
#include <memory>

class StateInGame : public State{
    public:
        StateInGame();
        ~StateInGame();
        void Update();
        void Render();
        States GetState() { return State::States::INGAME; };


    private:
        shared_ptr<EventManager> eventManager;
        shared_ptr<Car> car;
        shared_ptr<GameObject> ground;
        shared_ptr<Camera> cam;
        shared_ptr<ManPowerUp> manPowerUps;
        shared_ptr<RenderFacadeManager> renderFacadeManager;
        shared_ptr<InputFacadeManager> inputFacadeManager;
        shared_ptr<PhysicsFacadeManager> physicsFacadeManager;
        shared_ptr<RenderFacade> renderEngine;
        shared_ptr<InputFacade> inputEngine;
        shared_ptr<PhysicsFacade> physicsEngine;

        int lastFPS = -1;
        uint32_t then;
	
};


#endif  // STATEINGAME_H 