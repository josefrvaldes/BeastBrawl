#include "entities/hero.h"


int main()
{
    Hero *h = new Hero();
    cout << "Nuestro hero tiene position? " << h->HasComponent(CompType::Position) << endl;

    CPosition *pos = new CPosition();
    h->AddComponent(*pos);

    cout << "Nuestro hero tiene position? " << h->HasComponent(CompType::Position) << endl;
}