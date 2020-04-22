#include "GameOptions.h"

#include <algorithm>
#include <iostream>

GameOptions* GameOptions::instance = 0;

GameOptions* GameOptions::GetInstance() {
    if (instance == 0) {
        instance = new GameOptions();
    }
    return instance;
}

void GameOptions::SetCharacter(int c) {

    mainCharacter = c;

    //Randomiza los personajes de la IA cuando te coges un personaje nuevo
    std::vector<int> personajes = {0, 1, 2, 3, 4, 5};
    std::random_shuffle(personajes.begin(), personajes.end());
    auto mCharacter = GameOptions::GetInstance()->GetCharacter();
    for(size_t i = 0; i < personajes.size(); ++i) {
        if (personajes[i] == mCharacter) {
            auto aux = personajes.back();
            personajes.back() = personajes[i];
            personajes[i] = aux;
            break;
        }
    }

    for(size_t i = 0; i < iaCharacter.size(); ++i) {
        iaCharacter[i] = personajes[i];
    }

    /*std::cout << "+++++++++++++++ PERSONAJES " << std::endl;
    std::cout << "MAIN: " << mainCharacter << std::endl;
    std::cout << "IAs: ";
    for(auto i=0;i<iaCharacter.size(); ++i) {
        std::cout << iaCharacter[i] << " ";
    }
    std::cout << "\n";*/
}