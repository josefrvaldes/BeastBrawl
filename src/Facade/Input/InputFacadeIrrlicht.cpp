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

void InputFacadeIrrlicht::CheckInputLobbyMultiConnecting() {

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
	return false;
};