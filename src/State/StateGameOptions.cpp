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
 
 
 #include "StateGameOptions.h"

#include <iostream>

StateGameOptions::StateGameOptions(){

    //std::cout << "> GAME OPTIONS constructor" << std::endl;

    renderEngine = RenderFacadeManager::GetInstance()->GetRenderFacade();
    renderEngine->FacadeInitGameOptions();

    InitState();

}

void StateGameOptions::InitState() {

    if (!soundEngine){
        soundEngine = SoundFacadeManager::GetInstance()->GetSoundFacade();
    }
    soundEngine->SetState(3);
}


void StateGameOptions::Render(){
    renderEngine->FacadeBeginScene();
    renderEngine->FacadeDrawGameOptions();
    renderEngine->FacadeEndScene();
}


void StateGameOptions::Input(){
    renderEngine->FacadeCheckInputGameOptions();
}


void StateGameOptions::Update(){
    EventManager::GetInstance().Update();
}

StateGameOptions::~StateGameOptions(){
    renderEngine->FacadeReleaseOptions();
}