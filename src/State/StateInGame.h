#ifndef STATEINGAME_H
#define STATEINGAME_H

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
#include "../Entities/BoxPowerUp.h"
#include "../Entities/Totem.h"
#include "../Entities/WayPoint.h"
#include "../Managers/ManPowerUp.h"
#include "../Managers/ManWayPoint.h"
#include "../EventManager/EventManager.h"
#include "../Facade/Input/InputFacadeManager.h"
#include "../Facade/Physics/PhysicsFacadeManager.h"
#include "../Facade/Render/RenderFacadeManager.h"
#include "../Facade/Sound/SoundFacade.h"
#include "../Facade/Sound/SoundFacadeManager.h"
#include "../Systems/PhysicsPowerUp.h"
#include "../Systems/SystemBoxPowerUp.h"
#include "../Systems/Collisions.h"
#include "../Systems/Physics.h"
#include "../Systems/PhysicsAI.h"
#include "../Game.h"
#include "../Managers/ManCar.h"
#include "../Managers/ManPowerUp.h"
#include "../Managers/ManBoxPowerUp.h"
#include "../Managers/ManWayPoint.h"
#include "../Managers/ManTotem.h"
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
    shared_ptr<ManBoxPowerUp> manBoxPowerUps;
    shared_ptr<ManCar> manCars;
    shared_ptr<RenderFacadeManager> renderFacadeManager;
    shared_ptr<InputFacadeManager> inputFacadeManager;
    shared_ptr<PhysicsFacadeManager> physicsFacadeManager;
    shared_ptr<RenderFacade> renderEngine;
    shared_ptr<InputFacade> inputEngine;
    shared_ptr<PhysicsFacade> physicsEngine;
    shared_ptr<Collisions> collisions;
    shared_ptr<ManWayPoint> manWayPoint;
    shared_ptr<ManTotem> manTotems;
    //shared_ptr<SoundFacadeManager> soundFacadeManager;
    SoundFacade *soundEngine;
    shared_ptr<Physics> physics;
    shared_ptr<PhysicsPowerUp> phisicsPowerUp;
    shared_ptr<SystemBoxPowerUp> sysBoxPowerUp;
    shared_ptr<float> deltaTime;

    shared_ptr<Totem> totem;

    shared_ptr<PhysicsAI> physicsAI;
    int lastFPS = -1;
    //uint32_t then;
    time_point<system_clock> then;
    vector<float> deltas;
    float CalculateDelta(float);
};

#endif  // STATEINGAME_H