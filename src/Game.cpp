#include "Game.h"

using namespace std;

Game* Game::game = 0;
Game* Game::GetInstance() {
    if(game==0){
        game = new Game();
    }
    return game;
}


void Game::SetState(State::States stateType) {
    switch (stateType) {
        case State::INTRO:
            //currentState = new StateIntro();
            break;
        case State::MENU:
            currentState = make_shared<StateMenu>();
            gameStarted = false;
            break;
        case State::CONTROLS:
            //currentState = new StateControls();
            break;
        case State::CREDITS:
            //currentState = new StateCredits();
            break;
        case State::MAP:
            //currentState = new StateMap();
            break;
        case State::INGAME:
            if(!gameStarted){
                currentState = make_shared<StateInGame>();
                gameState = currentState;
                gameStarted = true;
            }else{
                currentState = gameState;
            }
            break;
        case State::PAUSE:
            currentState = make_shared<StatePause>();
            break;
        case State::ENDRACE:
            currentState = make_shared<StateEndRace>();
            break;
        default:
            cout << "This state doesn't exist" << endl;
    }

    // Inicializa los bancos cada vez que se cambia de estado.
    currentState->InitState();
}

void Game::InitGame() {
    // To-Do put window values
    renderFacadeManager = RenderFacadeManager::GetInstance();
    renderFacadeManager->InitializeIrrlicht();

    inputFacadeManager = InputFacadeManager::GetInstance();
    inputFacadeManager->InitializeIrrlicht();

    physicsFacadeManager = PhysicsFacadeManager::GetInstance();
    physicsFacadeManager->InitializeIrrlicht();
}

void Game::MainLoop() {

    renderFacadeManager->GetRenderFacade()->FacadeSetWindowCaption("Beast Brawl");

    SoundFacadeManager* soundFacadeManager = SoundFacadeManager::GetInstance();

    //Inicializa la fachada de FMOD.
    soundFacadeManager->InitializeFacadeFmod();
    soundFacadeManager->GetSoundFacade()->InitSoundEngine();

    while (renderFacadeManager->GetRenderFacade()->FacadeRun()) {
        currentState->Input();
        currentState->Update();

        //Actualiza el motor de audio.
        soundFacadeManager->GetSoundFacade()->Update();
        currentState->Render();
    }

    //Libera los sonidos y bancos.
    soundFacadeManager->GetSoundFacade()->TerminateSoundEngine();
    
    renderFacadeManager->GetRenderFacade()->FacadeDeviceDrop();
}