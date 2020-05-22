/**
 * Beast Brawl
 * Game created as a final project of the Multimedia Engineering Degree in the University of Alicante.
 * Made by Clover Games Studio, with members 
 * Carlos de la Fuente Torres delafuentetorresc@gmail.com,
 * Antonio Jose Martinez Garcia https://www.linkedin.com/in/antonio-jose-martinez-garcia/,
 * Jesús Mas Carretero jmasc03@gmail.com, 
 * Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/, 
 * Rubén Rubio Martínez https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/, 
 * and Jose Valdés Sirvent https://www.linkedin.com/in/jose-f-valdés-sirvent-6058b5a5/ github -> josefrvaldes
 * 
 * 
 * @author Clover Games Studio
 * 
 */
 
 
 #include "GameValues.h"

#include <algorithm>
#include <iostream>

GameValues* GameValues::instance = 0;

GameValues* GameValues::GetInstance() {
    if (instance == 0) {
        instance = new GameValues();
    }
    return instance;
}
 
void GameValues::SetCharacter(int c) {

    mainCharacter = c;

    //Randomiza los personajes de la IA cuando te coges un personaje nuevo
    std::vector<int> personajes = {0, 1, 2, 3, 4, 5};
    std::random_shuffle(personajes.begin(), personajes.end());
    auto mCharacter = GameValues::GetInstance()->GetCharacter();
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