#ifndef STATEMENU_H
#define STATEMENU_H


#include "../Game.h"


class StateMenu : public State{
    public:
        StateMenu();
        virtual ~StateMenu();
        void Update();
        void Render();
        virtual States GetState() { return State::States::MENU; };


    private:
	
};


#endif  // STATEMENU_H 