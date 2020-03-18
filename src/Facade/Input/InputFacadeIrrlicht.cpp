#include "InputFacadeIrrlicht.h"
#include <Constants.h>

InputFacadeIrrlicht::InputFacadeIrrlicht(){
    //eventManager = EventManager::GetInstance();
	//RenderFacadeManager::GetInstance().GetRenderFacade()->AddReceiver(receiver);
}

vector<Constants::InputTypes> InputFacadeIrrlicht::CheckInput(){
    //Data d;
	//d.gameObject = &go;

	//if(receiver->IsKeyDown(KEY_KEY_W)){
    //    eventManager->AddEventMulti(Event {EventType::PRESS_I,d});
	//}else if(receiver->IsKeyDown(KEY_KEY_A)){
    //    eventManager->AddEventMulti(Event {EventType::PRESS_A,d});
	//}else if(receiver->IsKeyDown(KEY_KEY_D)){
    //    eventManager->AddEventMulti(Event {EventType::PRESS_D,d});
	//}
	return vector<Constants::InputTypes>();
}


void InputFacadeIrrlicht::CheckInputMenu(){
}


void InputFacadeIrrlicht::CheckInputPause(){
}


void InputFacadeIrrlicht::CheckInputEndRace(){
}



InputFacadeIrrlicht::~InputFacadeIrrlicht(){
	
}
