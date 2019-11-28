#include "InputFacadeIrrlicht.h"

InputFacadeIrrlicht::InputFacadeIrrlicht(){
    eventManager = EventManager::GetInstance();
	//RenderFacadeManager::GetInstance().GetRenderFacade()->AddReceiver(receiver);
}

void InputFacadeIrrlicht::CheckInputs(Entity& go){
    Data d;
	d.gameObject = &go;

	if(receiver->IsKeyDown(KEY_KEY_W)){
        eventManager->AddEvent(Event {EventType::PRESS_W,d});
	}else if(receiver->IsKeyDown(KEY_KEY_A)){
        eventManager->AddEvent(Event {EventType::PRESS_A,d});
	}else if(receiver->IsKeyDown(KEY_KEY_D)){
        eventManager->AddEvent(Event {EventType::PRESS_D,d});
	}

}

MyEventReceiver* InputFacadeIrrlicht::GetReceiver(){
	return receiver;
}

void InputFacadeIrrlicht::SetDevice(uintptr_t device_ptr){
	device = reinterpret_cast<IrrlichtDevice*>(device_ptr);
	device->setEventReceiver(receiver);
}

InputFacadeIrrlicht::~InputFacadeIrrlicht(){
	
}
