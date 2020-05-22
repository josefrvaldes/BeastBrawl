/**
 * Beast Brawl
 * Game created as a final project of the Multimedia Engineering Degree in the University of Alicante.
 * Made by Clover Games Studio, with members 
 * Carlos de la Fuente Torres delafuentetorresc@gmail.com,
 * Antonio Jose Martinez Garcia https://www.linkedin.com/in/antonio-jose-martinez-garcia/,
 * Jesús Mas Carretero jmasc03@gmail.com, 
 * Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/, 
 * Rubén Rubio Martínez https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/, 
 * and Jose Valdés Sirvent https://www.linkedin.com/in/jose-f-valdés-sirvent-6058b5a5/ github -> josefrvaldes
 * 
 * 
 * @author Clover Games Studio
 * 
 */
 
 
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
