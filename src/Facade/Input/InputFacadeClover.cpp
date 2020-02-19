#include "InputFacadeClover.h"

InputFacadeClover::InputFacadeClover(){
    //eventManager = EventManager::GetInstance();
	//RenderFacadeManager::GetInstance().GetRenderFacade()->AddReceiver(receiver);
    cout << "Creado el input de Clover\n"; 
    
    

}

void InputFacadeClover::CheckInput(){
    device->InputClose();
}


void InputFacadeClover::CheckInputMenu(){
}


void InputFacadeClover::CheckInputPause(){
}


void InputFacadeClover::CheckInputEndRace(){
}



InputFacadeClover::~InputFacadeClover(){
	
}
