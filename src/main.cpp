#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <thread>
// #include "../include/boost/asio/io_service.hpp"
#include "Game.h"

#include "Online/UDPClient.h"

using namespace std;

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