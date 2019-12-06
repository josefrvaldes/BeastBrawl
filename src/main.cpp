#include "Game.h"
#include "lib/glm/vec3.hpp"
#include <iostream>
#include <memory>
#include <map>

using namespace std;

class Pointer{
    public:
    Pointer(int _a){
        a = _a;
        cout << "Creado\n";
    }

    ~Pointer(){
        cout << "Eliminado\n";
    }

    int a;
};
int main()
{
    
    
    map<string,shared_ptr<Pointer>> mapa;
    shared_ptr<Pointer> ptr = make_shared<Pointer>(5);

    mapa["hola"] = ptr;

    try
    {
       Game *game = Game::GetInstance();

    
        game->SetState(State::States::INGAME);
        game->InitGame();
    }
    catch(...)
    {
        std::cout << "Algo ha fallado" << std::endl;
    }
    
    
	
	return 0;
}