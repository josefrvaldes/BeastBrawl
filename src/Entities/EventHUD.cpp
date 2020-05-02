#include "EventHUD.h"

#include "../Components/CEventHUD.h"

EventHUD::EventHUD(){
    shared_ptr<CEventHUD> cEventHUD = make_shared<CEventHUD>();
    AddComponent(cEventHUD);
}
