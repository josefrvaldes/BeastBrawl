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
 
 
 #include "ManHUDEvent.h"

#include <Components/CEventHUD.h>


ManHUDEvent::ManHUDEvent() {
    eventShow = make_unique<EventHUD>();
    SubscribeToEvents();
}

void ManHUDEvent::SubscribeToEvents() {
    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::SET_EVENT_HUD,
        bind(&ManHUDEvent::ActiveEvent, this, placeholders::_1),
        "ActiveEvent"));
}

void ManHUDEvent::SetEventHUD(eventHUDType type, uint16_t pj) {
    auto cEventHudComp = static_cast<CEventHUD*>(eventShow->GetComponent(CompType::EventHudComp).get());
    if (cEventHudComp) {
        cEventHudComp->characterEventHUD = pj;
        cEventHudComp->typeEvent = type;
        switch ((int)type) {
            case 1: cEventHudComp->spriteTypeEvent = "media/stoleHUD.png"; break;
            case 2: cEventHudComp->spriteTypeEvent = "media/catchHUD.png"; break;
            case 3: cEventHudComp->spriteTypeEvent = "media/loseHUD.png"; break;
            default: break;
        }
        cEventHudComp->timeStart = system_clock::now();
        eventActive = true;
    }
}

void ManHUDEvent::ActiveEvent(DataMap* d) {
    auto _pj = any_cast<uint16_t>((*d)[DataType::NUM]);
    auto _type = any_cast<int>((*d)[DataType::TYPE]);
    eventHUDType type;
    switch (_type) {
        case 1: type = eventHUDType::STOLE; break;
        case 2: type = eventHUDType::CATCH; break;
        case 3: type = eventHUDType::LOSE;  break;
        default: type = eventHUDType::NONE; break;
    }
    SetEventHUD(type, _pj);
}