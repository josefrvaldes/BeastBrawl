#pragma once

#include <chrono>
#include <cstdint>
#include <iostream>
#include <list>
#include <memory>
#include <string>

#include "../../lib/glm/vec3.hpp"
#include "../Entities/Camera.h"
#include "../Entities/Car.h"
#include "../Entities/CarAI.h"
#include "../Entities/GameObject.h"
#include "../Entities/PowerUp.h"
#include "../Entities/WayPoint.h"
#include "../EventManager/EventManager.h"
#include "../Facade/Input/InputFacadeManager.h"
#include "../Facade/Physics/PhysicsFacadeManager.h"
#include "../Facade/Render/RenderFacadeManager.h"
#include "../Facade/Sound/SoundFacadeManager.h"
#include "../Facade/Sound/SoundFacade.h"
#include "../Game.h"
#include "../Managers/ManCar.h"
#include "../Managers/ManCarAI.h"
#include "../Managers/ManPowerUp.h"
#include "../Managers/ManWayPoint.h"
#include "../Systems/Physics.h"
#include "../Systems/PhysicsAI.h"
#include "../behaviourTree/behaviourTree.h"
#include "../behaviourTree/decorator.h"
#include "../behaviourTree/selector.h"
#include "../behaviourTree/sequence.h"
#include "../fuzzyLogic/fuzzyLogic.h"

using namespace std;
using namespace chrono;

class StateInGame : public State {
   public:
    StateInGame();
    ~StateInGame();
    void InitState() override;
    void Input() override;
    void Update() override;
    void Render() override;
    States GetState() { return State::States::INGAME; };

   private:
    shared_ptr<EventManager> eventManager;
    shared_ptr<GameObject> ground;
    shared_ptr<Camera> cam;
    shared_ptr<ManPowerUp> manPowerUps;
    shared_ptr<ManCar> manCars;
    RenderFacadeManager* renderFacadeManager;
    InputFacadeManager* inputFacadeManager;
    PhysicsFacadeManager* physicsFacadeManager;
    RenderFacade* renderEngine;
    InputFacade* inputEngine;
    PhysicsFacade* physicsEngine;
    shared_ptr<SoundFacadeManager> soundFacadeManager;
    SoundFacade *soundEngine;
    shared_ptr<Physics> physics;
    shared_ptr<float> deltaTime;

    shared_ptr<CarAI> carAI;
    shared_ptr<ManWayPoint> manWayPoint;
    shared_ptr<PhysicsAI> physicsAI;
    int lastFPS = -1;
    //uint32_t then;
    time_point<system_clock> then;
    vector<float> deltas;
    float CalculateDelta(float);
};
