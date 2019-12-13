#pragma once

#include "State/State.h"
#include "State/StateMenu.h"
#include "State/StateInGame.h"
#include "Facade/Render/RenderFacadeManager.h"
#include "Facade/Input/InputFacadeManager.h"
#include "Facade/Physics/PhysicsFacadeManager.h"

#include <iostream>
#include <memory>
using namespace std;

class Game{
    public:
        virtual ~Game(){};
        static Game* GetInstance();
        void SetState(State::States stateType);
        shared_ptr<State> GetState() { return currentState; };
        shared_ptr<State> GetLastState() { return lastState; };
        void InitGame();
        void MainLoop();
        



    private:
        Game(){};
        static Game* game;
        //State::States ActualState;
        shared_ptr<State> currentState;
        shared_ptr<State> gameState;
        shared_ptr<State> lastState;
        RenderFacadeManager* renderFacadeManager;
        InputFacadeManager* inputFacadeManager;
        PhysicsFacadeManager* physicsFacadeManager;

        bool gameStarted = false;
};

