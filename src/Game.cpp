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
 
 
 #include "Game.h"

#include "Facade/Physics/PhysicsFacadeManager.h"

#include "State/StateInit.h"
#include "State/StateSelectCharacter.h"
#include "State/StateGameOptions.h"
#include "State/StateTournamentOptions.h"
#include "State/StateEndRace.h"
#include "State/StateEndTournament.h"
#include "State/StateInGameMulti.h"
#include "State/StateInGameSingle.h"
#include "State/StateLobbyMulti.h"
#include "State/StateMenu.h"
#include "State/StatePause.h"
#include "State/StateControls.h"
#include "State/StateCredits.h"
#include "State/StateSettings.h"

#include <Constants.h>
#include "GameValues.h"


using namespace std;

Game* Game::game = 0;
Game* Game::GetInstance() {
    if (game == 0) {
        game = new Game();
    }
    return game;
}

void Game::SetState(State::States stateType) {
    //cout << "GAME inicia estado nuevo" << endl;

    switch (stateType) {
        case State::INTRO:
            EventManager::GetInstance().ClearEvents();
            EventManager::GetInstance().ClearListeners();
            currentState = make_shared<StateInit>();
            gameState.reset();
            SuscribeEvents();
            gameStarted = false;
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
        case State::SELECT_CHARACTER:
            EventManager::GetInstance().ClearEvents();
            EventManager::GetInstance().ClearListeners();
            currentState = make_shared<StateSelectCharacter>();
            gameState.reset();
            SuscribeEvents();
            gameStarted = false;
            break;
        case State::GAME_OPTIONS:
            EventManager::GetInstance().ClearEvents();
            EventManager::GetInstance().ClearListeners();
            currentState = make_shared<StateGameOptions>();
            gameState.reset();
            SuscribeEvents();
            gameStarted = false;
            break;
        case State::TOURNAMENT_OPTIONS:
            EventManager::GetInstance().ClearEvents();
            EventManager::GetInstance().ClearListeners();
            currentState = make_shared<StateTournamentOptions>();
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
            EventManager::GetInstance().ClearEvents();
            EventManager::GetInstance().ClearListeners();
            currentState = make_shared<StateCredits>();
            gameState.reset();
            SuscribeEvents();
            gameStarted = false;
            break;
        case State::SETTINGS:
            EventManager::GetInstance().ClearEvents();
            EventManager::GetInstance().ClearListeners();
            currentState = make_shared<StateSettings>();
            gameState.reset();
            SuscribeEvents();
            gameStarted = false;
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
            gameState.reset();
            SuscribeEvents();
            gameStarted = false;
            break;
        case State::ENDTOURNAMENT:
            EventManager::GetInstance().ClearEvents();
            EventManager::GetInstance().ClearListeners();
            currentState = make_shared<StateEndTournament>();
            gameState.reset();
            SuscribeEvents();
            gameStarted = false;
            break;
        case State::LOBBY_MULTI:
            currentState = make_shared<StateLobbyMulti>();
            break;
        default:
            //cout << "This state doesn't exist" << endl;
            break;
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
    

    //Se inicia GameOptions
    GameValues::GetInstance();

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
            EventType::STATE_INTRO,
            bind(&Game::SetStateIntro, this, placeholders::_1),
            "StateIntro"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::STATE_MENU,
        bind(&Game::SetStateMenu, this, placeholders::_1),
        "StateMenu"));


    EventManager::GetInstance().SubscribeMulti(Listener(
            EventType::STATE_SELECT_CHARACTER,
            bind(&Game::SetStateSelectCharacter, this, placeholders::_1),
            "StateSelectCharacter"));


    EventManager::GetInstance().SubscribeMulti(Listener(
            EventType::STATE_GAME_OPTIONS,
            bind(&Game::SetStateGameOptions, this, placeholders::_1),
            "StateGameOptions"));
        
    EventManager::GetInstance().SubscribeMulti(Listener(
            EventType::STATE_TOURNAMENT_OPTIONS,
            bind(&Game::SetStateTournamentOptions, this, placeholders::_1),
            "StateTournamentOptions"));

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
        "StateEndRace in Game.cpp"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::STATE_ENDTOURNAMENT,
        bind(&Game::SetStateEndTournament, this, placeholders::_1),
        "StateEndTournament in Game.cpp"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::STATE_LOBBYMULTI,
        bind(&Game::SetStateLobbyMulti, this, placeholders::_1),
        "SetStateLobbyMulti"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::STATE_CONTROLS,
        bind(&Game::SetStateControls, this, placeholders::_1),
        "SetStateControls"));

    EventManager::GetInstance().SubscribeMulti(Listener(
            EventType::STATE_CREDITS,
            bind(&Game::SetStateCredits, this, placeholders::_1),
            "SetStateCredits"));

    EventManager::GetInstance().SubscribeMulti(Listener(
            EventType::STATE_SETTINGS,
            bind(&Game::SetStateSettings, this, placeholders::_1),
            "SetStateSettings"));
}

void Game::MainLoop() {
    SoundFacadeManager* soundFacadeManager = SoundFacadeManager::GetInstance();

    RenderFacadeManager* renderFacadeMan = RenderFacadeManager::GetInstance();

    int lastFPS = renderFacadeMan->GetRenderFacade()->FacadeGetFPS();
    int frameCount = 0;

    if (Constants::RENDER_ENGINE == Constants::RenderEngine::CLOVER) {
        lastFPS = renderFacadeMan->GetRenderFacade()->FacadeGetTime();
    }

    while (renderFacadeMan->GetRenderFacade()->FacadeRun()) {
        currentState->Input();
        currentState->Update();
        renderFacadeMan->GetRenderFacade()->FacadeUpdateViewport();

        //Actualiza el motor de audio.
        soundFacadeManager->GetSoundFacade()->Update();
        currentState->Render();

        if (Constants::RENDER_ENGINE == Constants::RenderEngine::IRRLICHT) {
            int fps = renderFacadeMan->GetRenderFacade()->FacadeGetFPS();
            if(lastFPS != fps) {
                renderFacadeMan->GetRenderFacade()->FacadeSetWindowCaption("Beast Brawl", fps);
                lastFPS = fps;
                frameCount = 0;
            }
        } else {
            int currentTime = renderFacadeMan->GetRenderFacade()->FacadeGetTime();
            frameCount++;
            if ( currentTime - lastFPS >= 1.0 ) {
                renderFacadeMan->GetRenderFacade()->FacadeSetWindowCaption("Beast Brawl", frameCount);
                //cout << frameCount << endl;
                frameCount = 0;
                lastFPS = currentTime;
            }
        }

    }

    renderFacadeMan->GetRenderFacade()->FacadeDeviceDrop();

    //cout << "Game Main Loop" << endl;
}

void Game::TerminateGame() {
    //Libera los sonidos y bancos.
    SoundFacadeManager::GetInstance()->GetSoundFacade()->Terminate();
    //cout << "**********************************************" << endl;
    //cout << "Game Terminate" << endl;
}

//Funciones del EventManager

void Game::SetStateIntro(DataMap* d) {
    SetState(State::INTRO);
}

void Game::SetStateMenu(DataMap* d) {
    SetState(State::MENU);
}

void Game::SetStateSelectCharacter(DataMap* d) {
    SetState(State::SELECT_CHARACTER);
}

void Game::SetStateGameOptions(DataMap* d) {
    SetState(State::GAME_OPTIONS);
}

void Game::SetStateTournamentOptions(DataMap* d) {
    SetState(State::TOURNAMENT_OPTIONS);
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
    vector<uint8_t> charactersOnline = any_cast<vector<uint8_t>>((*d)[DataType::CHARACTERS_ONLINE]);
    if (!gameStarted) {
        shared_ptr<State> newState;
        newState = make_shared<StateInGameMulti>(IdOnline, vectorIdOnline, charactersOnline);
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

void Game::SetStateEndTournament(DataMap* d) {
    SetState(State::ENDTOURNAMENT);
}

void Game::SetStateLobbyMulti(DataMap* d) {
    SetState(State::LOBBY_MULTI);
}

void Game::SetStateControls(DataMap* d){
    SetState(State::CONTROLS);
}

void Game::SetStateCredits(DataMap* d){
    SetState(State::CREDITS);
}

void Game::SetStateSettings(DataMap *d) {
    SetState(State::SETTINGS);
}