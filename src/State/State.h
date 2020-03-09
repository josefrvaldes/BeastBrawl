#pragma once


#include <iostream>
using namespace std;

class State{
    public:
        enum States{ NO_STATE, INTRO, MENU, MAP, INGAME_SINGLE, INGAME_MULTI, ENDRACE, PAUSE, CONTROLS, CREDITS, LOBBY_MULTI};
        State();
        virtual ~State();
        virtual void InitState() = 0;
        virtual void Update() = 0;
        virtual void Input() = 0;
        virtual void Render() = 0;
        virtual States GetState() = 0;
        virtual States GetEstado() { return States::NO_STATE; };


    private:

};
