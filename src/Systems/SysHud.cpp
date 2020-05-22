#include "SysHud.h"

#include <Entities/EventHUD.h>
#include <Managers/ManHUDEvent.h>

using namespace std;

void SysHud::UpdateEventHud(ManHUDEvent* manHudEvent) {
    if (manHudEvent && manHudEvent->IsEventHUDActive()) {
        auto& event = manHudEvent->GetEventShow();
        if (event.get()) {
            auto cEventHud = static_cast<CEventHUD*>(event.get()->GetComponent(CompType::EventHudComp).get());
            if(cEventHud) {
                cEventHud->accumulatedTime += duration_cast<milliseconds>(system_clock::now() - cEventHud->timeStart).count();
                cEventHud->timeStart = system_clock::now();
                //cout << "ACTUALIZO TIEMPO: " << cEventHud->accumulatedTime/1000 << endl;
                
                if((cEventHud->accumulatedTime)/1000 >= cEventHud->DURATION_TIME){
                    manHudEvent->SetEventActive(false);
                    cEventHud->accumulatedTime = 0;
                }
            }
        }
    }
}

void SysHud::UpdateMinimap(std::vector<std::shared_ptr<Entity>>& entities) {

}