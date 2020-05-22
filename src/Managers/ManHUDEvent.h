#pragma once

#include "Manager.h"
#include <chrono>
#include <Entities/EventHUD.h>
#include <EventManager/Event.h>
#include <EventManager/EventManager.h>
#include <Components/CEventHUD.h>

class ManHUDEvent : public Manager {

    public:
        ManHUDEvent();
        ~ManHUDEvent() = default;

        void SetEventHUD(eventHUDType, uint16_t);
        void SetEventActive(bool b)                 { eventActive = b; };

        bool IsEventHUDActive()                     { return eventActive; };
        shared_ptr<EventHUD>& GetEventShow()        { return eventShow; };

    private:
        void SubscribeToEvents() override;
        void ActiveEvent(DataMap*);

        bool eventActive { false };

        shared_ptr<EventHUD> eventShow;

};
