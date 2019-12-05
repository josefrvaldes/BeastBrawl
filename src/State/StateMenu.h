#ifndef STATEMENU_H
#define STATEMENU_H


#include "../Game.h"


class StateMenu : public State{
    public:
        StateMenu();
        ~StateMenu();
        void Update();
        void Render();
        States GetState() { return State::States::MENU; };


    private:
	
};


#endif  // STATEMENU_H 