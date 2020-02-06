#include "Game.h"
//#include <src/Systems/Utils.h>

using namespace std;

int main() {

    // Utils::RunSerializationTest2();

//    unsigned char b{0};
//    b |= 1UL << 0;
//    std::cout << +b << std::endl;
//    b |= 1UL << 1;
//    b |= 1UL << 3;
//    bool bit = (b >> 7) & 1U;
//    std::cout << bit << std::endl;
//    b |= 1UL << 7;
//    b &= ~(1UL << 0);
//    std::cout << +b << std::endl;
//
//    bit = (b >> 7) & 1U;
//    std::cout << bit << std::endl;

    try {
    Game* game = Game::GetInstance();

    game->InitGame();
    game->SetState(State::States::MENU);
    game->MainLoop();
    game->TerminateGame();
    } catch (exception &e) {
        cout << "Algo ha fallado " << e.what() << endl;
    }

    return 0;
}