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

#include "State/State.h"
#include <Aliases.h>

#include <iostream>
#include <memory>

using namespace std;

class State;

class Game{
    public:
        virtual ~Game(){};
        static Game* GetInstance();
        void SetState(State::States stateType);
        shared_ptr<State> GetState() { return currentState; };
        shared_ptr<State> GetLastState() { return lastState; };
        void InitGame();
        void MainLoop();
        void TerminateGame();


        //EventManager
        void SetStateIntro(DataMap* d);
        void SetStateMenu(DataMap* d);
        void SetStateSelectCharacter(DataMap* d);
        void SetStateGameOptions(DataMap* d);
        void SetStateTournamentOptions(DataMap* d);
        void SetStatePause(DataMap* d);
        void SetStateInGameSingle(DataMap* d);
        void SetStateInGameMulti(DataMap* d);
        void SetStateEndRace(DataMap* d);
        void SetStateEndTournament(DataMap* d);
        void SetStateLobbyMulti(DataMap* d);
        void SetStateControls(DataMap* d);
        void SetStateCredits(DataMap* d);
        void SetStateSettings(DataMap* d);


    private:
        Game()= default;;
        static Game* game;
        void SuscribeEvents();

        shared_ptr<State> currentState;
        shared_ptr<State> gameState;
        shared_ptr<State> lastState;

        bool gameStarted = false;
};

