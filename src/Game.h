#pragma once

#include "State/State.h"
#include "State/StateMenu.h"
#include "State/StateInGame.h"
#include "Facade/Render/RenderFacadeManager.h"

#include <iostream>
#include <memory>
using namespace std;

class Game{
    public:
        Game(){};
        virtual ~Game(){};
        static shared_ptr<Game> GetInstance();
        void SetState(State::States stateType);
        shared_ptr<State> GetState() { return currentState; };
        shared_ptr<State> GetLastState() { return lastState; };
        void InitGame();
        int i;
        



    private:
        static const shared_ptr<Game> game;
        //State::States ActualState;
        shared_ptr<State> currentState;
        shared_ptr<State> lastState;

        void MainLoop();
};

