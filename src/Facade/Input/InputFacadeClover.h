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
#include <memory>
#include <GameValues.h>

#include "InputFacade.h"
#include "../Render/RenderFacadeManager.h"
#include "../Sound/SoundFacadeManager.h"

#include "../../../CLEngine/src/CLEngine.h"

using namespace std;
using namespace CLE;

class InputFacadeClover : public InputFacade{
    public:
        InputFacadeClover();
        ~InputFacadeClover() override = default;

        bool IsConectedGamepad() override;

        void CheckInputIntro() override;
        void CheckInputMenu(int&, int)    override;

        void CheckInputSelectCharacter(int&, int) override;
        void CheckInputGameOptions(std::vector<int>&, int[], int&) override;
        void CheckInputTournamentOptions(std::vector<int>&, int[], int&) override;

        bool ShowHud(bool) override;
        void ShowTable(Constants::ShowTableMinimap&) override;
        bool CheckInputSingle()  override;
        void CheckInputLobbyMultiConnecting() override;
        void CheckInputLobbyMultiWait() override;
        void CheckInputLobbyMultiExit() override;
        void CheckInputLobbyMultiSelChar(int &, int) override;
        vector<Constants::InputTypes> CheckInputMulti()override;

        void CheckInputPause(int&, int)   override;
        void CheckInputEndRace(int&, int, bool) override;
        void CheckInputEndTournament(int&, int, uint8_t) override;
        void CheckInputController() override;
        void CheckInputCredits() override;
        void CheckInputSettings(std::vector<int> &inputs, int maxInputs[], int& option) override;
        bool CheckInputAnimationsStartEnd() override;

        bool InTournament() override {return tournamentMode;};
        void SetWindowContext(CLEngine* d) {device = d;}
        void PressedA() override {SetValueInput(BUTTON_A, true);}

    private:
        void CheckInputUp(std::vector<int>&, int&);
        void CheckInputDown(std::vector<int>&, int&);
        void CheckInputLeft(std::vector<int>&, int[], int&);
        void CheckInputRight(std::vector<int>&, int[], int&);
        void CheckInputBack();

        void CheckInputSelCharUpDown(int&);
        void CheckInputSelCharLeft(int&, int);
        void CheckInputSelCharRight(int&, int);
        void CheckInputSelCharIntro(int&);

        bool CheckAnyKey();

        CLEngine* device;

        enum InputXBox{BUTTON_A, BUTTON_B, BUTTON_X, BUTTON_Y, BUTTON_LB, BUTTON_RB, BUTTON_BACK, BUTTON_START, BUTTON_XBOX, BUTTON_STICK_UP, BUTTON_STICK_DOWN, BUTTON_STICK_L, BUTTON_STICK_R, END};
        bool pressF1 { false };
        std::unordered_map<InputXBox, bool> inputsPressed;
        core::array<SJoystickInfo> joystickInfo;
        bool multiplayer { false };
        bool tournamentMode { false };

        bool IsInputPressed(InputXBox input);
        void SetValueInput(InputXBox input, bool valuePressed);
        bool IsKeyOrGamepadPress(int key, int button1, bool axes, float axe, int button2, bool doubleGamepad);
        bool IsKeyPress(int key);
        bool IsGamepadPress(int key);
        bool IsGamepadAxisPress(int button1, float axes);
        bool HasDelayPassed();

        bool WeHaveToGoToMenu {false}; // esto sirve para que se oiga el sonidito de pasar desde pausa ingame hasta menú, es una chapuza pero no quiero perder más tiempo en esa chorrada
        uint64_t timerGoToMenu {0};

        void ChangeGameOptions(int, int);
        void ChangeTournamentOptions(int, int, int);
        void ChangeSettings(int, int);
};