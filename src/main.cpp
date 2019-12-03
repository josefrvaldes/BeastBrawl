#include "Game.h"
#include "lib/glm/vec3.hpp"
#include <iostream>

using namespace std;


int main()
{

    Game *game = Game::GetInstance();

    
    game->SetState(State::States::INGAME);
    game->InitGame();
	
	return 0;
}