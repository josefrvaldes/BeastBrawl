#include "Game.h"
#include "lib/glm/vec3.hpp"
#include <iostream>

using namespace std;

class Position{
    public:
    Position(glm::vec3 _pos){
        position = _pos;
    }

    glm::vec3 position;
};

int main()
{
    glm::vec3 pos = glm::vec3(1.0f,2.0f,3.0f);
    Position* p = new Position(pos);
    cout << p->position.x << endl;
    p->position.x = 5.0f;
    cout << p->position.x << endl;

    Game *game = Game::GetInstance();

    
    game->SetState(State::States::INGAME);
    game->InitGame();
	
	return 0;
}