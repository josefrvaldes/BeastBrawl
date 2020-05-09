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
        virtual vector<Constants::InputTypes> CheckInputMulti()  = 0;
        virtual bool ShowTable(bool) = 0;
        virtual bool CheckInputSingle()  = 0;
        virtual void CheckInputMenu(int&, int)    = 0;
        virtual void CheckInputSelectCharacter(int&, int) = 0;
        virtual void CheckInputGameOptions(std::vector<int>&, int[], int&) = 0;
        virtual void CheckInputTournamentOptions(std::vector<int>&, int[], int&) = 0;
        virtual void CheckInputPause(int&, int)   = 0;
        virtual void CheckInputIntro() = 0;
        virtual void CheckInputEndRace(int&, int, bool) = 0;
        virtual void CheckInputEndTournament(int&, int, uint8_t) = 0;
        virtual void CheckInputLobbyMulti() = 0;
        virtual void CheckInputController() = 0;
        virtual void CheckInputCredits() = 0;
        virtual void CheckInputSettings(std::vector<int> &inputs, int maxInputs[], int& option) = 0;
        virtual bool CheckInputAnimationsStartEnd() = 0;

        virtual bool InTournament() = 0;


    protected:
        time_point<system_clock> timeStart;
        const int64_t inputDelay = 250;       // 0.5 segundos
        const int64_t inputDelayCamera = 250;       // 0.5 segundos
        bool invertedCam = false;
        bool totemCamActive = false;
        int idCarAIToDebug = 0;
};
