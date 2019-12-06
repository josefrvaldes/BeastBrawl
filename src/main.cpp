#include "Game.h"
#include "lib/glm/vec3.hpp"
#include <iostream>
#include <memory>

using namespace std;

// class Pointer{
//     public:
//     Pointer(int _a){
//         a = _a;
//         cout << "Creado\n";
//     }

//     ~Pointer(){
//         cout << "Eliminado\n";
//     }

//     int a;
// };
int main()
{
    
    // shared_ptr<Pointer> p1 = make_shared<Pointer>(5);
    // {

    //     {
    //         shared_ptr<Pointer> p2 = p1;
    //         cout << p1.use_count() << endl;
    //     }
    //     cout << p1.use_count() << endl;
    // }


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