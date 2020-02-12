#pragma once

#include <iostream>
#include <cstdint>
#include <string>
#include <EventManager/EventManager.h>
#include <EventManager/Event.h>
#include <Entities/Entity.h>


class InputFacade {
    public:
        virtual ~InputFacade() = default;
        virtual void CheckInput()        = 0;
        virtual void CheckInputMenu()    = 0;
        virtual void CheckInputPause()   = 0;
        virtual void CheckInputEndRace() = 0;

    protected:
};
