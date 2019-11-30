#include "Game.h"

int main()
{
    Game *game = Game::GetInstance();
    
    game->SetState(State::States::INGAME);
    game->InitGame();
	
	return 0;
}