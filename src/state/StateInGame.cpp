#include "include/StateInGame.h"
#include <iostream>

using namespace std;

StateInGame::StateInGame(){
    // constructor
    std::cout << "Estado InGame Creado" << std::endl;
}


StateInGame::~StateInGame(){
    // destructor
}



void StateInGame::Render(){

}


void StateInGame::Update(){
    std::cout << "Actualizado estado InGame" << std::endl;
}