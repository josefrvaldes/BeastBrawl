#include "Game.h"
#include <src/Systems/Utils.h>

using namespace std;

int main() {

    // Utils::RunSerializationTest2();

    // try {
    Game* game = Game::GetInstance();

    game->InitGame();
    game->SetState(State::States::MENU);
    game->MainLoop();
    game->TerminateGame();
    // } catch (exception &e) {
    //     cout << "Algo ha fallado " << e.what() << endl;
    // }

    return 0;
}