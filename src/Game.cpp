#include "Game.h"
#include "Facade/Physics/PhysicsFacadeManager.h"
#include "State/StateEndRace.h"
#include "State/StateInGameMulti.h"
#include "State/StateInGameSingle.h"
#include "State/StateLobbyMulti.h"
#include "State/StateMenu.h"
#include "State/StatePause.h"
#include "State/StateControls.h"
#include <Constants.h>


using namespace std;

Game* Game::game = 0;
Game* Game::GetInstance() {
    if (game == 0) {
        game = new Game();
    }
    return game;
}

void Game::SetState(State::States stateType) {
    cout << "GAME inicia estado nuevo" << endl;

    switch (stateType) {
        case State::INTRO:
            //currentState = new StateIntro();
            break;
        case State::MENU:
            //Al volver al menu todo el mundo se desuscribe o sea que volvemos a añadir las suscripciones
            EventManager::GetInstance().ClearEvents();
            EventManager::GetInstance().ClearListeners();
            currentState = make_shared<StateMenu>();
            gameState.reset();
            SuscribeEvents();
            gameStarted = false;
            break;
        case State::CONTROLS:
            EventManager::GetInstance().ClearEvents();
            EventManager::GetInstance().ClearListeners();
            currentState = make_shared<StateControls>();
            gameState.reset();
            SuscribeEvents();
            gameStarted = false;
            break;
        case State::CREDITS:
            //currentState = new StateCredits();
            break;
        case State::MAP:
            //currentState = new StateMap();
            break;
        case State::INGAME_SINGLE:
            if (!gameStarted) {
                shared_ptr<State> newState = make_shared<StateInGameSingle>();
                //cout << "Hemos creado el nuevo statInGameSingle" << endl;
                currentState = newState;
                //cout << "Hemos chafado el currentState con statInGameSingle" << endl;
                gameState = currentState;
                //cout << "Hemos chafado el gameState con statInGameSingle" << endl;
                gameStarted = true;
            } else {
                currentState = gameState;
                currentState->InitState();
            }
            break;
        case State::INGAME_MULTI:
            // ATENCIÓN: este estado se inicializa en un método abajo aparte
            // if (!gameStarted) {
            //     shared_ptr<State> newState = make_shared<StateInGameMulti>();
            //     cout << "Hemos creado el nuevo StateInGameMulti" << endl;
            //     currentState = newState;
            //     cout << "Hemos chafado el currentState con StateInGameMulti" << endl;
            //     gameState = currentState;
            //     cout << "Hemos chafado el gameState con StateInGameMulti" << endl;
            //     gameStarted = true;
            // } else {
            //     currentState = gameState;
            // }
            break;
        case State::PAUSE:
            currentState = make_shared<StatePause>();
            break;
        case State::ENDRACE:
            EventManager::GetInstance().ClearEvents();
            EventManager::GetInstance().ClearListeners();
            currentState = make_shared<StateEndRace>();
            if (gameMarkedToDelete) {
                gameState.reset();
                gameMarkedToDelete = false;
            } else
                gameMarkedToDelete = true;

            SuscribeEvents();
            gameStarted = false;
            break;
        case State::LOBBY_MULTI:
            currentState = make_shared<StateLobbyMulti>();
            break;
        default:
            cout << "This state doesn't exist" << endl;
    }

    // Inicializa los bancos cada vez que se cambia de estado.
    // Se cambia de sitio porque se tiene que hacer antes de iniciar los managers
    //currentState->InitState();
}

void Game::InitGame() {
    if(Constants::RENDER_ENGINE == Constants::RenderEngine::CLOVER){
        RenderFacadeManager::GetInstance()->InitializeClover();
        InputFacadeManager::GetInstance()->InitializeClover();
        PhysicsFacadeManager::GetInstance()->InitializeClover();
    }else if(Constants::RENDER_ENGINE == Constants::RenderEngine::IRRLICHT){
        RenderFacadeManager::GetInstance()->InitializeIrrlicht();
        InputFacadeManager::GetInstance()->InitializeIrrlicht();
        PhysicsFacadeManager::GetInstance()->InitializeIrrlicht();
    } 
    

    //Inicializa la fachada de FMOD.
    SoundFacadeManager::GetInstance()->InitializeFacadeFmod();
    SoundFacadeManager::GetInstance()->GetSoundFacade()->Initialize();

    SuscribeEvents();

    //cout << "Game Init" << endl;
    //cout << "**********************************************" << endl;
}

void Game::SuscribeEvents() {
    //cout << "Suscripciones\n";
    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::STATE_MENU,
        bind(&Game::SetStateMenu, this, placeholders::_1),
        "StateMenu"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::STATE_PAUSE,
        bind(&Game::SetStatePause, this, placeholders::_1),
        "StatePause"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::STATE_INGAMESINGLE,
        bind(&Game::SetStateInGameSingle, this, placeholders::_1),
        "StateInGameSingle"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::STATE_INGAMEMULTI,
        bind(&Game::SetStateInGameMulti, this, placeholders::_1),
        "StateInGameMulti"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::STATE_ENDRACE,
        bind(&Game::SetStateEndRace, this, placeholders::_1),
        "StateEndRace"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::STATE_LOBBYMULTI,
        bind(&Game::SetStateLobbyMulti, this, placeholders::_1),
        "SetStateLobbyMulti"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::STATE_CONTROLS,
        bind(&Game::SetStateControls, this, placeholders::_1),
        "SetStateControls"));
}

void Game::MainLoop() {
    SoundFacadeManager* soundFacadeManager = SoundFacadeManager::GetInstance();

    RenderFacadeManager* renderFacadeMan = RenderFacadeManager::GetInstance();

    int lastFPS = -1;

    while (renderFacadeMan->GetRenderFacade()->FacadeRun()) {
        currentState->Input();
        currentState->Update();

        //Actualiza el motor de audio.
        soundFacadeManager->GetSoundFacade()->Update();
        currentState->Render();

        int fps = renderFacadeMan->GetRenderFacade()->FacadeGetFPS();
        if(lastFPS != fps) {
            renderFacadeMan->GetRenderFacade()->FacadeSetWindowCaption("Beast Brawl", fps);
            lastFPS = fps;
        }
    }

    renderFacadeMan->GetRenderFacade()->FacadeDeviceDrop();

    cout << "Game Main Loop" << endl;
}

void Game::TerminateGame() {
    //Libera los sonidos y bancos.
    SoundFacadeManager::GetInstance()->GetSoundFacade()->Terminate();
    cout << "**********************************************" << endl;
    cout << "Game Terminate" << endl;
}

//Funciones del EventManager

void Game::SetStateMenu(DataMap* d) {
    cout << "LLEGA\n";
    SetState(State::MENU);
}

void Game::SetStatePause(DataMap* d) {
    SetState(State::PAUSE);
}

void Game::SetStateInGameSingle(DataMap* d) {
    shared_ptr<DataMap> data = make_shared<DataMap>();

    (*data)[TYPE_POWER_UP] = typeCPowerUp::None;

    EventManager::GetInstance().AddEventMulti(Event{EventType::UPDATE_POWERUP_HUD, data});
    SetState(State::INGAME_SINGLE);
}

void Game::SetStateInGameMulti(DataMap* d) {
    //SetState(State::INGAME_MULTI);
    uint16_t IdOnline = any_cast<uint16_t>((*d)[DataType::ID_ONLINE]);
    vector<uint16_t> vectorIdOnline = any_cast<vector<uint16_t>>((*d)[DataType::VECTOR_ID_ONLINE]);
    if (!gameStarted) {
        shared_ptr<State> newState;
        newState = make_shared<StateInGameMulti>(IdOnline, vectorIdOnline);
        currentState = newState;
        gameState = currentState;
        gameStarted = true;
    } else {
        currentState = gameState;
    }

    currentState->InitState();
}

void Game::SetStateEndRace(DataMap* d) {
    SetState(State::ENDRACE);
}

void Game::SetStateLobbyMulti(DataMap* d) {
    SetState(State::LOBBY_MULTI);
}

void Game::SetStateControls(DataMap* d){
    SetState(State::CONTROLS);
}
