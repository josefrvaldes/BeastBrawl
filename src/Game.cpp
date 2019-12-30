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
    RenderFacadeManager::GetInstance()->InitializeIrrlicht();
    InputFacadeManager::GetInstance()->InitializeIrrlicht();
    PhysicsFacadeManager::GetInstance()->InitializeIrrlicht();

    //Inicializa la fachada de FMOD.
    SoundFacadeManager::GetInstance()->InitializeFacadeFmod();
    SoundFacadeManager::GetInstance()->GetSoundFacade()->InitSoundEngine();
}

void Game::MainLoop() {

    SoundFacadeManager* soundFacadeManager = SoundFacadeManager::GetInstance();

    RenderFacadeManager* renderFacadeMan = RenderFacadeManager::GetInstance();
    renderFacadeMan->GetRenderFacade()->FacadeSetWindowCaption("Beast Brawl");

    while (renderFacadeMan->GetRenderFacade()->FacadeRun()) {
        currentState->Input();
        currentState->Update();

        //Actualiza el motor de audio.
        soundFacadeManager->GetSoundFacade()->Update();
        currentState->Render();
    }
    
    renderFacadeMan->GetRenderFacade()->FacadeDeviceDrop();
}

void Game::TerminateGame() {

    //Libera los sonidos y bancos.
    SoundFacadeManager::GetInstance()->GetSoundFacade()->TerminateSoundEngine();
}