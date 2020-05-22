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