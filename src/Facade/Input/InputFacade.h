#pragma once

#include <iostream>
#include <cstdint>
#include <string>
#include <chrono>

#include <EventManager/EventManager.h>
#include <EventManager/Event.h>
#include <Entities/Entity.h>

using namespace chrono;


class InputFacade {
    public:
        virtual ~InputFacade() = default;
        virtual void CheckInput()        = 0;
        virtual void CheckInputMenu()    = 0;
        virtual void CheckInputPause()   = 0;
        virtual void CheckInputEndRace() = 0;

    protected:
        time_point<system_clock> timeStart;
        const int64_t inputDelay = 500;       // 0.5 segundos
        const int64_t inputDelayCamera = 250;       // 0.5 segundos
        bool invertedCam = false;
        bool totemCamActive = false;
        int idCarAIToDebug = 0;
};
