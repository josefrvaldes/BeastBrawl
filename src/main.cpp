#include "entities/hero.h"
#include "components/position.h"
#include "components/speed.h"
#include "systems/physics.h"
#include "behaviourTree/behaviourTree.h"
#include "behaviourTree/selector.h"
#include "behaviourTree/sequence.h"

#include "include/Game.h"

#include <iostream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////
//                           COMPROBAR BEHAVIOR TREE
////////////////////////////////////////////////////////////////////////////////////////////////////
bool door = false; // imaginemos que door=false es = puerta cerrada "door is open?"
bool key  = true;  // tenemos una llave
// ACCION DE ABRIR LA PUERTA
struct openDoor : public behaviourTree { 
		virtual bool run() override {
            door = true;
            cout << "Abrimos la puerta" << endl;
            return true;
		}
};
//ACCION COGER LA LLAVE
struct getKey : public behaviourTree { 
		virtual bool run() override {
            key = true;
            cout << "Cogemos la llave" << endl;
            return true;
		}
};
//CONDICION PUERTA ABIERTA?
struct haveDoorOpen : public behaviourTree { 
		virtual bool run() override {
            cout << "Comprobamos si la puerta esta abierta: " << boolalpha << door << endl;
            return door;
		}
};
//CONDICION TENEMOS LLAVE?
struct haveKey : public behaviourTree { 
		virtual bool run() override {
            cout << "Comprobamos si tenemos la llave: " << boolalpha << key << endl;
			return  key;
		}
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


int main()
{
    Game *game = Game::GetInstance();
    
    game->SetState(State::States::MENU);
    game->InitGame();




    //BehaviorTree BASICO
                    // SELECTOR1
                        // 
        ///////////////////////////////////////////////////////////////////////////////////
        //                                      //                                       //
// La pueta esta abierta?                     SEQUENCE                           // coger llave
                                    ///////////////////////////////
                                    //                          //
                                // tengo llave?             //abrir puerta


    selector* selector1 = new selector;
    sequence* sequence1 = new sequence;

    haveDoorOpen* puertaAbiertaSiNo = new haveDoorOpen;
    haveKey* tengoLlaveSiNo = new haveKey;
    openDoor* abrirPuerta = new openDoor;
    getKey* cogerLlave = new getKey;

    selector1->addChild(puertaAbiertaSiNo);
    selector1->addChild(sequence1);
    selector1->addChild(cogerLlave);

    sequence1->addChild(tengoLlaveSiNo);
    sequence1->addChild(abrirPuerta);

	cout << "--------------------" << endl;
    while (!selector1->run()){} // If the operation starting from the root fails, keep trying until it succeeds.
	cout << "--------------------" << endl;



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