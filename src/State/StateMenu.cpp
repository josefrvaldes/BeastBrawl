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
 
 
 #include "StateMenu.h"
#include <iostream>

using namespace std;

StateMenu::StateMenu(){

    //std::cout << "> MENU constructor" << std::endl;
    renderEngine = RenderFacadeManager::GetInstance()->GetRenderFacade();
    
    InitState();
    renderEngine->FacadeInitMenu();


}

// Cargamos los bancos de sonido Menu.
void StateMenu::InitState() {

    if (!soundEngine){
        soundEngine = SoundFacadeManager::GetInstance()->GetSoundFacade();
    }
    soundEngine->SetState(1);
}


void StateMenu::Render(){
    renderEngine->FacadeBeginScene();
    renderEngine->FacadeDrawMenu();
    renderEngine->FacadeEndScene();
}


void StateMenu::Input(){
    renderEngine->FacadeCheckInputMenu();
}


void StateMenu::Update(){
    EventManager::GetInstance().Update();
}

StateMenu::~StateMenu(){
    renderEngine->FacadeReleaseMenu();
}