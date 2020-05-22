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
 
 
 #pragma once


#include <iostream>
using namespace std;

class State{
    public:
        enum States{ NO_STATE, INTRO, MENU, SELECT_CHARACTER, GAME_OPTIONS, TOURNAMENT_OPTIONS, INGAME_SINGLE, INGAME_MULTI, ENDRACE, ENDTOURNAMENT, 
                    PAUSE, CONTROLS, CREDITS, LOBBY_MULTI, SETTINGS};
        State();
        virtual ~State() = 0;
        virtual void InitState() = 0;
        virtual void Update() = 0;
        virtual void Input() = 0;
        virtual void Render() = 0;
        virtual States GetState() = 0;
        virtual States GetEstado() { return States::NO_STATE; };


    private:

};
