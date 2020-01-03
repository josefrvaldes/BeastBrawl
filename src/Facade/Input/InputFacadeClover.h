#pragma once

#include <iostream>
#include <memory>
#include "InputFacade.h"
//#include "../../../include/sdl/SDL.h"
#include <SDL2/SDL.h>

using namespace std;

class InputFacadeClover : public InputFacade{
    public:
        InputFacadeClover();
        ~InputFacadeClover();
        void CheckInput()        override;
        void CheckInputMenu()    override;
        void CheckInputPause()   override;
        void CheckInputEndRace() override;

    private:
        SDL_Window *device = nullptr;
        SDL_GameController *gamepad = nullptr;


};