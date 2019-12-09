#ifndef STATEMENU_H
#define STATEMENU_H


#include "../Game.h"


class StateMenu : public State{
    public:
        StateMenu();
        ~StateMenu() = default;
        void Input() override;
        void Update() override;
        void Render() override;
        States GetState() { return State::States::MENU; };


    private:
	
};


#endif  // STATEMENU_H 