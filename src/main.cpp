#include <iostream>
#include <memory>
#include "Game.h"

#include "CLPhysics/CLPhysics.h"

using namespace std;

int main() {
    //CLPhysics::RunTests();
    try {
        shared_ptr<Game> game = Game::GetInstance();

        game->SetState(State::States::INGAME);
        game->InitGame();
    } catch (...) {
        cout << "Algo ha fallado" << endl;
    }

    return 0;
}