#ifndef GAME_H
#define GAME_H

#include <iostream>
#include "State.h"


class Game{
    public:
        static Game* GetInstance();
        void SetState(State::States stateType);
        State* GetState() { return currentState; };
        State* GetLastState() { return lastState; };
        void InitGame();
        



    private:
        static Game *game;
        //State::States ActualState;
        State *currentState;
        State *lastState;

        Game();
        virtual ~Game();
        void MainLoop();
};


#endif // GAME_H