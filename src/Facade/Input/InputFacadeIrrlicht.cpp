#include "InputFacadeIrrlicht.h"
#include <Constants.h>

InputFacadeIrrlicht::InputFacadeIrrlicht(){
    //eventManager = EventManager::GetInstance();
	//RenderFacadeManager::GetInstance().GetRenderFacade()->AddReceiver(receiver);
}

vector<Constants::InputTypes> InputFacadeIrrlicht::CheckInputMulti(){
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

bool InputFacadeIrrlicht::CheckInputSingle(){
	return false;
}

void InputFacadeIrrlicht::CheckInputIntro() {

}

void InputFacadeIrrlicht::CheckInputLobbyMulti() {

}

void InputFacadeIrrlicht::CheckInputController() {

}

void InputFacadeIrrlicht::CheckInputMenu(int&, int){
}

void InputFacadeIrrlicht::CheckInputSelectCharacter(int &, int) {

}

void InputFacadeIrrlicht::CheckInputGameOptions(std::vector<int>&, int[], int&) {

}

void InputFacadeIrrlicht::CheckInputPause(int&, int){
}


void InputFacadeIrrlicht::CheckInputEndRace(int&, int, bool){
}

void InputFacadeIrrlicht::CheckInputCredits() {

}

void InputFacadeIrrlicht::CheckInputSettings(std::vector<int> &inputs, int *maxInputs, int &option) {

}


bool InputFacadeIrrlicht::CheckInputAnimationsStartEnd() {
	
};