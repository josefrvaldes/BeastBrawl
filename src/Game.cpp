#include "Game.h"

using namespace std;

const shared_ptr<Game> Game::game = make_shared<Game>();
shared_ptr<Game> Game::GetInstance() {
    //static EventManager instance;
    // if(instance==nullptr){
    //     instance = make_shared<EventManager>();
    // }
    game->i = 23;
    return game;
}

void Game::SetState(State::States stateType) {
    switch (stateType) {
        case State::INTRO:
            //currentState = new StateIntro();
            break;
        case State::MENU:
            currentState = make_shared<StateMenu>();
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
            currentState = make_shared<StateInGame>();
            break;
        case State::ENDRACE:
            //currentState = new StateEndRace();
            break;
        default:
            cout << "This state doesn't exist" << endl;
    }
}

void Game::InitGame() {
    // To-Do put window values
    MainLoop();
}

void Game::MainLoop() {
    shared_ptr<RenderFacadeManager> renderFacadeManager = RenderFacadeManager::GetInstance();

    renderFacadeManager->GetRenderFacade()->FacadeSetWindowCaption("Beast Brawl");

    //Lo creo aqui porque queria llamar al TerminateSoundEngine despues del bucle.
    SoundFacadeManager* soundFacadeManager = SoundFacadeManager::GetInstance();

    //Si se incluye esta funcion en el constructor de SoundFacadeFMOD da violacion de segmento.
    soundFacadeManager->InitializeFacadeFmod();
    soundFacadeManager->GetSoundFacade()->InitSoundEngine();

    currentState->InitState();

    while (renderFacadeManager->GetRenderFacade()->FacadeRun()) {
        currentState->Input();
        currentState->Update();
        soundFacadeManager->GetSoundFacade()->Update();
        currentState->Render();
    }
    soundFacadeManager->GetSoundFacade()->TerminateSoundEngine();
    renderFacadeManager->GetRenderFacade()->FacadeDeviceDrop();
}