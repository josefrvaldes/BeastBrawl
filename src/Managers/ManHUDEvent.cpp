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