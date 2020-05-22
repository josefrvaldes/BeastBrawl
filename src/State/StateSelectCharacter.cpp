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
 
 
 #include "StateSelectCharacter.h"

#include <iostream>

using namespace std;

StateSelectCharacter::StateSelectCharacter(){
    //std::cout << "> SELECT CHARECTER constructor" << std::endl;
    renderEngine = RenderFacadeManager::GetInstance()->GetRenderFacade();
    InitState();
    renderEngine->FacadeInitSelectCharacter();
}

void StateSelectCharacter::InitState() {

    if (!soundEngine){
        soundEngine = SoundFacadeManager::GetInstance()->GetSoundFacade();
    }
    soundEngine->SetState(2);
}


void StateSelectCharacter::Render(){
    renderEngine->FacadeBeginScene();
    renderEngine->FacadeDrawSelectCharacter();
    renderEngine->FacadeDrawAll();
    renderEngine->FacadeEndScene();
}


void StateSelectCharacter::Input(){
    renderEngine->FacadeCheckInputSelectCharacter();
}


void StateSelectCharacter::Update(){
    EventManager::GetInstance().Update();
}

StateSelectCharacter::~StateSelectCharacter(){
    //std::cout << "> SELECT CHARECTER destructor" << std::endl;
}