#pragma once

#include "State/State.h"
#include <Aliases.h>

#include <iostream>
#include <memory>

using namespace std;

class State;

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
        void SetStateControls(DataMap* d);
        
    



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

