#pragma once

#include <iostream>
#include <cstdint>
#include <string>
#include "../../EventManager/EventManager.h"
#include "../../EventManager/Event.h"
#include "../../Entities/Entity.h"


class InputFacade {
    public:
        virtual ~InputFacade() = default;
        virtual void CheckInputs(Entity&) = 0;
        virtual void SetDevice(uintptr_t) = 0;

    protected:
        EventManager* eventManager;
};
