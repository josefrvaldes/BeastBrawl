#include "StateInGame.h"
#include <iostream>

using namespace std;

StateInGame::StateInGame(){
    // constructor
    std::cout << "Estado InGame Creado" << std::endl;
    //init();
}

//StateInGame::init() {
    //ManagerPowerUps.init();
    //ManagerCoches.init();
//
    //// esto sería dentro del manager de coches
    //manager::init() {
    //    for(Coche c : coches) {
    //        c = new Coche();  
    //        // dentro del constructor sería algo así
    //        Coche::Cohe() {
    //            ponerComponentes();
    //            suscribirAEventos();
    //            
    //        }  
    //    }
    //}
//}

StateInGame::~StateInGame(){
    // destructor
}



void StateInGame::Render(){

}


void StateInGame::Update(){
    std::cout << "Actualizado estado InGame" << std::endl;
}