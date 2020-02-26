#include <iostream>
#include "Game.h"

using namespace std;

int main() {

    //try {
    Game* game = Game::GetInstance();

    game->InitGame();
    game->SetState(State::States::INGAME_SINGLE);
    game->MainLoop();
    game->TerminateGame();
    //} catch (exception &e) {
    //    cout << "Algo ha fallado " << e.what() << endl;
    //}

    return 0;
}