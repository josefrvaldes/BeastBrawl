#include <Systems/Utils.h>
#include <iostream>
#include "Game.h"
#include "Constants.h"

using namespace std;

int main(int argc, char *argv[]) {

    if(argc == 2) {
        Constants::SERVER_HOST = argv[1];
    }
    cout << "La ip del servidor será " << Constants::SERVER_HOST << endl;

    //try {
    Game* game = Game::GetInstance();

    game->InitGame();

    if(Constants::RENDER_ENGINE == Constants::RenderEngine::CLOVER){
        game->SetState(State::States::INGAME_SINGLE);
        
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