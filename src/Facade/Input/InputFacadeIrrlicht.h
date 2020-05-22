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

#include "InputFacade.h"

#include <iostream>
#include <memory>
#include <irrlicht/irrlicht.h>

using namespace std;
using namespace irr;


class MyEventReceiver : public IEventReceiver{

    public:
        // This is the one method that we have to implement
        virtual bool OnEvent(const SEvent& event)
        {
            // Remember whether each key is down or up
            if (event.EventType == EET_KEY_INPUT_EVENT)
                KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

            if(event.EventType == irr::EET_JOYSTICK_INPUT_EVENT && event.JoystickEvent.Joystick == 0)
                JoystickState = event.JoystickEvent;
            return false;
        }

        // This is used to check whether a key is being held down
        virtual bool IsKeyDown(EKEY_CODE keyCode) const
        {
            return KeyIsDown[keyCode];
        }
        
        const SEvent::SJoystickEvent & GetJoyStickState(void) const{
            return JoystickState;
        }

        MyEventReceiver()
        {
            for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
                KeyIsDown[i] = false;
        }

    private:
        // We use this array to store the current state of each key
        bool KeyIsDown[KEY_KEY_CODES_COUNT];
        SEvent::SJoystickEvent JoystickState;
};


class InputFacadeIrrlicht : public InputFacade{
    public:
        InputFacadeIrrlicht();
        ~InputFacadeIrrlicht() override = default;
        bool IsConectedGamepad() override { return false; };
        vector<Constants::InputTypes> CheckInputMulti() override;
        bool ShowHud(bool inputTable) override { return inputTable;};
        void ShowTable(Constants::ShowTableMinimap&) override {};
        bool CheckInputSingle() override;
        void CheckInputMenu(int&, int) override;
        void CheckInputSelectCharacter(int&, int) override;
        void CheckInputGameOptions(std::vector<int>&, int[], int&) override;
        void CheckInputTournamentOptions(std::vector<int>&, int[], int&) override {};
        void CheckInputPause(int&, int) override;
        void CheckInputIntro() override;
        void CheckInputEndRace(int&, int, bool) override;
        void CheckInputEndTournament(int&, int, uint8_t) override {};
        void CheckInputLobbyMultiConnecting() override;
        void CheckInputLobbyMultiWait() override {};
        void CheckInputLobbyMultiExit() override {};
        void CheckInputLobbyMultiSelChar(int &, int) override {};
        void CheckInputController() override;
        void CheckInputCredits() override;
        void CheckInputSettings(std::vector<int> &inputs, int maxInputs[], int& option) override;
        bool CheckInputAnimationsStartEnd() override;
        bool InTournament() override {return false;};
        void PressedA() override {};

    private:
        MyEventReceiver receiver;
		IrrlichtDevice *device;

};
