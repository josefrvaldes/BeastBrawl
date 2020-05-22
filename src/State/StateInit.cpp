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
 
 
 #include "StateInit.h"

#include <iostream>

using namespace std;

StateInit::StateInit(){

    //std::cout << "> INIT constructor" << std::endl;

    renderEngine = RenderFacadeManager::GetInstance()->GetRenderFacade();
    inputEngine = InputFacadeManager::GetInstance()->GetInputFacade();
    renderEngine->FacadeInitIntro();
    InitState();

}

void StateInit::InitState() {

    if (!soundEngine){
        soundEngine = SoundFacadeManager::GetInstance()->GetSoundFacade();
    }
    soundEngine->SetState(0);

}


void StateInit::Render(){
    renderEngine->FacadeBeginScene();
    renderEngine->FacadeDrawIntro();
    renderEngine->FacadeEndScene();
}


void StateInit::Input(){
    inputEngine->CheckInputIntro();
    renderEngine->FacadeCheckInputIntro();
}


void StateInit::Update(){
    EventManager::GetInstance().Update();
}