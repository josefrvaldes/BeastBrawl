#include <iostream>
#include <thread>
#include "Game.h"


int main() {

    try {
        Game* game = Game::GetInstance();

        game->InitGame();
        game->SetState(State::States::MENU);
        game->MainLoop();
        game->TerminateGame();
    } catch (...) {
        cout << "Algo ha fallado" << endl;
    }

    return 0;
}