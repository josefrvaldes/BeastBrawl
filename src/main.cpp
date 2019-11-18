#include "entities/hero.h"
#include "components/position.h"
#include "components/speed.h"
#include "systems/physics.h"

#include "Game.h"



int main()
{
    Game *game = Game::GetInstance();
    
    game->SetState(State::States::INGAME);
    game->InitGame();

    return 0;
    /*
    // creamos a nuestro hero
    Hero *hero = new Hero();
    cout << "Nuestro hero tiene position? " << hero->HasComponent(CompType::Position) << endl;


    // creamos nuestro componente position
    CPosition *pos = new CPosition(1.2, 4.3, -3.1);
    hero->AddComponent(pos);

    cout << "Nuestro hero tiene position? " << hero->HasComponent(CompType::Position) << endl;

    // ahora obtenemos la posición a partir de la entidad que hemos creado
    //CPosition *posObtenida = (CPosition *) h->getComponents().at(CompType::Position); // alternativa 2 a obtener el componente
    CPosition *posObtenida = (CPosition *) hero->getComponents()[CompType::Position];
    cout << "La posición de nuestro hero es " << *posObtenida << endl;

    // También se podría hacer de una forma más compacta directamente usando el hero
    cout << "La posición de nuestro hero es " << *(CPosition *)(hero->getComponents()[CompType::Position]) << endl;

    
    // ahora creamos un componente velocidad
    CSpeed *speed = new CSpeed(2.7, 7.1, -1.1);

    // y se la agregamos a nuestro hero igual que antes
    hero->AddComponent(speed);

    // y la mostramos por pantalla
    cout << "La velocidad de nuestro hero es " << *(CPosition *)(hero->getComponents()[CompType::Speed]) << endl;


    // Ahora movemos a nuestro hero usando un sistema physics
    // requiere que le mandemos el componente speed y el componente position, y él hace el cálculo.
    // como veis, no necesita de la entidad, por tanto es totalmente reutilizable para cualquier 
    // entidad que tenga speed y position
    Physics::move(
        (CSpeed *)hero->getComponents()[CompType::Speed], 
        (CPosition *)hero->getComponents()[CompType::Position]
    );

    // y mostramos su nueva posición por pantalla
    cout << "La posición de nuestro hero es " << *(CPosition *)(hero->getComponents()[CompType::Position]) << endl;


    delete speed;
    delete posObtenida;
    delete hero;*/
}