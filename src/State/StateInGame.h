#ifndef STATEINGAME_H
#define STATEINGAME_H


#include "../Game.h"


class StateInGame : public State{
    public:
        StateInGame();
        virtual ~StateInGame();
        void Update();
        void Render();
        virtual States GetState() { return State::States::INGAME; };


    private:
	
};


#endif  // STATEINGAME_H 