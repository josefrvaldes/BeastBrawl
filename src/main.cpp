#include "Game.h"
#include "lib/glm/vec3.hpp"
#include <iostream>
#include <memory>
#include <map>

using namespace std;

int main()
{

    try
    {
       shared_ptr<Game> game = Game::GetInstance();

    
        game->SetState(State::States::INGAME);
        game->InitGame();
    }
    catch(...)
    {
        std::cout << "Algo ha fallado" << std::endl;
    }
    
    
	
	return 0;
}