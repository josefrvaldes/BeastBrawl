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
 
 
 #include <Systems/Utils.h>
#include <Components/CBufferOnline.h>
#include <iostream>
#include "Game.h"
#include "Constants.h"
#include <chrono>
#include <thread>

using namespace std;

int main(int argc, char *argv[]) {

    if(argc == 2) {
        Constants::ANIM_ACTIVATED = std::stoi(argv[1]);
    }

    if(argc == 3) {
        Constants::ANIM_ACTIVATED = std::stoi(argv[1]);
        Constants::SERVER_HOST = argv[2];
    }

    //try {
    Game* game = Game::GetInstance();

    game->InitGame();

    if(Constants::RENDER_ENGINE == Constants::RenderEngine::CLOVER){
        game->SetState(State::States::INTRO);
        
    }else if(Constants::RENDER_ENGINE == Constants::RenderEngine::IRRLICHT){
        game->SetState(State::States::MENU);
    }
    //game->SetState(State::States::INGAME_SINGLE);
    game->MainLoop();
    game->TerminateGame();
    //} catch (exception &e) {
    //    cout << "Algo ha fallado " << e.what() << endl;
    //}

    return 0;
}