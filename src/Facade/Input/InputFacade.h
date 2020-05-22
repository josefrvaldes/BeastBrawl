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
#include <cstdint>
#include <string>
#include <chrono>
#include <vector>

#include <EventManager/EventManager.h>
#include <EventManager/Event.h>
#include <Entities/Entity.h>
#include <Constants.h>

using namespace chrono;


class InputFacade {
    public:
        virtual ~InputFacade() = default;
        virtual bool IsConectedGamepad() = 0;
        virtual vector<Constants::InputTypes> CheckInputMulti()  = 0;
        virtual bool ShowHud(bool) = 0;
        virtual void ShowTable(Constants::ShowTableMinimap&) = 0;
        virtual bool CheckInputSingle()  = 0;
        virtual void CheckInputMenu(int&, int)    = 0;
        virtual void CheckInputSelectCharacter(int&, int) = 0;
        virtual void CheckInputGameOptions(std::vector<int>&, int[], int&) = 0;
        virtual void CheckInputTournamentOptions(std::vector<int>&, int[], int&) = 0;
        virtual void CheckInputPause(int&, int)   = 0;
        virtual void CheckInputIntro() = 0;
        virtual void CheckInputEndRace(int&, int, bool) = 0;
        virtual void CheckInputEndTournament(int&, int, uint8_t) = 0;
        virtual void CheckInputLobbyMultiConnecting() = 0;
        virtual void CheckInputLobbyMultiWait() = 0;
        virtual void CheckInputLobbyMultiExit() = 0;
        virtual void CheckInputLobbyMultiSelChar(int &, int) = 0;
        virtual void CheckInputController() = 0;
        virtual void CheckInputCredits() = 0;
        virtual void CheckInputSettings(std::vector<int> &inputs, int maxInputs[], int& option) = 0;
        virtual bool CheckInputAnimationsStartEnd() = 0;

        virtual bool InTournament() = 0;
        virtual void PressedA() = 0;


    protected:
        time_point<system_clock> timeStart;
        const int64_t inputDelay = 250;       // 0.5 segundos
        const int64_t inputDelayCamera = 250;       // 0.5 segundos
        bool invertedCam = false;
        bool totemCamActive = false;
        int idCarAIToDebug = 0;
};
