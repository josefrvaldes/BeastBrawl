#pragma once

#include "../../EventManager/EventManager.h"
#include "../../EventManager/Event.h"
#include "../../Entities/Entity.h"

class PhysicsFacade{
    public:
        virtual ~PhysicsFacade() = default;
        virtual void Update(Entity*, Entity*) = 0; //TODO: Ahora le paso valores pero luego accedera al array de entidades
    protected:
        EventManager* eventManager;
};