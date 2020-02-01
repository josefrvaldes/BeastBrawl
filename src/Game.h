#pragma once

#include "State/State.h"
#include "State/StateMenu.h"
#include "State/StateInGame.h"
#include "State/StatePause.h"
#include "State/StateEndRace.h"
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
        void TerminateGame();


        //EventManager
        void SetStateMenu(DataMap* d);
        void SetStatePause(DataMap* d);
        void SetStateInGameSingle(DataMap* d);
        void SetStateInGameMulti(DataMap* d);
        void SetStateEndRace(DataMap* d);
        void SetStateLobbyMulti(DataMap* d);
        
    

    private:
        Game(){};
        static Game* game;
        void SuscribeEvents();
        
        shared_ptr<State> currentState;
        shared_ptr<State> gameState;
        shared_ptr<State> lastState;

        bool gameStarted = false;
        bool gameMarkedToDelete {false};
};

