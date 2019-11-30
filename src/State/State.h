#pragma once


#include <iostream>


class State{
    public:
        enum States{ NO_STATE, INTRO, MENU, MAP, INGAME, ENDRACE, PAUSE, CONTROLS, CREDITS};
        State();
        virtual ~State();
        virtual void Update() {};
        virtual void Render() {};
        virtual States GetEstado() { return States::NO_STATE; };


    private:

};
