#ifndef STATEINGAME_H
#define STATEINGAME_H

#include <cstdint>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <chrono>

#include "../../lib/glm/vec3.hpp"
#include "../Entities/Camera.h"
#include "../Entities/Car.h"
#include "../Entities/GameObject.h"
#include "../Entities/PowerUp.h"
#include "../Entities/WayPoint.h"
#include "../Entities/CarAI.h"
#include "../Systems/Physics.h"
#include "../Managers/ManPowerUp.h"
#include "../Managers/ManWayPoint.h"
#include "../Managers/ManCarAI.h"
#include "../EventManager/EventManager.h"
#include "../Facade/Input/InputFacadeManager.h"
#include "../Facade/Physics/PhysicsFacadeManager.h"
#include "../Facade/Render/RenderFacadeManager.h"
#include "../Game.h"
#include "../Managers/ManCar.h"
#include "../Managers/ManPowerUp.h"
#include "../Systems/Physics.h"
#include "../Systems/PhysicsAI.h"
#include "../fuzzyLogic/fuzzyLogic.h"
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
    shared_ptr<RenderFacadeManager> renderFacadeManager;
    shared_ptr<InputFacadeManager> inputFacadeManager;
    shared_ptr<PhysicsFacadeManager> physicsFacadeManager;
    shared_ptr<RenderFacade> renderEngine;
    shared_ptr<InputFacade> inputEngine;
    shared_ptr<PhysicsFacade> physicsEngine;
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

#endif  // STATEINGAME_H