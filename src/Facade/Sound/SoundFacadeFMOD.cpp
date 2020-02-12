#include <cstdlib>
#include <string>
#include <iostream>

#include "SoundFacadeFMOD.h"

using namespace std;

/*
 * FMOD ERRORS
 */

#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

void ERRCHECK_fn(FMOD_RESULT result, const char* file, int line) {
    if (result != FMOD_OK) {
        cerr << file << " (Linea: " << line << "): FMOD HA CASCAO"
             << " -> " << FMOD_ErrorString(result) << " Codigo de error: " << result << endl;
        exit(-1);
    }
}

/*
 * SOUND FACADE FMOD
 */

SoundFacadeFMOD::~SoundFacadeFMOD() {
    delete coreSystem;
    delete system;
    delete masterBank;
    delete stringsBank;
}

/*
 * Inicializa FMOD Studio, inicializando tambien FMOD Core.
 */
void SoundFacadeFMOD::InitSoundEngine() {
    ERRCHECK(FMOD::Studio::System::create(&system));
    ERRCHECK(system->getCoreSystem(&coreSystem));
    ERRCHECK(coreSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0));
    ERRCHECK(system->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0));

    LoadMasterBank();
    std::cout << "***** Sound engine ON" << endl;
}

/*
 * Libera los audios y bancos de sonido.
 */
void SoundFacadeFMOD::TerminateSoundEngine() {
    std::cout << "----- Voy a mandar a Sound engine a la shit" << endl;
    UnloadAllBanks();
    UnloadMasterBank();

    ERRCHECK(system->release());
    std::cout << "***** Sound engine K.O." << endl;
}

/*
 * Carga el banco Master y el MasterStrings.
 */
void SoundFacadeFMOD::LoadMasterBank() {
    ERRCHECK(system->loadBankFile("./media/fmod/Master.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank));
    ERRCHECK(system->loadBankFile("./media/fmod/Master.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank));
}

/*
 * Libera el banco Master y MasterStrings.
*/
void SoundFacadeFMOD::UnloadMasterBank() {
    ERRCHECK(stringsBank->unload());
    ERRCHECK(masterBank->unload());
    std::cout << "***** Master bank a la shit" << endl;
}

/*
 * Libera los sonidos que se han cargado en memoria y los bancos.
 */
void SoundFacadeFMOD::UnloadAllBanks() {
    for (auto instance : eventInstances) {
        ERRCHECK(instance.second->release());
    }
    eventInstances.clear();

    soundDescriptions.clear();

    for (auto bank : banks) {
        ERRCHECK(bank.second->unload());
    }
    banks.clear();
    std::cout << "***** Bancos y sonidos liberados" << endl;
}

/**
 * Crea la instancia de sonido
 */
FMOD::Studio::EventInstance* SoundFacadeFMOD::CreateInstance(const string& nameEvent) {
    FMOD::Studio::EventInstance* instance = nullptr;
    ERRCHECK(soundDescriptions[nameEvent]->createInstance(&instance));
    return instance;
}

/*
 * Libera los bancos que habian cargados anteriormente, si habian.
 * Carga los nuevos bancos y se suscribe a los nuevos eventos.
 * @param numState - Estado en el que nos encontramos.
 */
void SoundFacadeFMOD::SetState(const uint8_t numState) {
    LoadSoundByState(numState);
    SubscribeToGameEvents(numState);
}

/* 
 * Se suscribe a los eventos segun el estado en el que se encuentra el juego.
 * @param numState - Estado en el que nos encontramos.
 */
void SoundFacadeFMOD::SubscribeToGameEvents(const uint8_t numState) {
    switch (numState) {
        case 1:  // Intro
            break;
        case 2:  // Menu
            break;
        case 3:  // Map
            break;
        case 4:  // InGame
            EventManager::GetInstance().SubscribeMulti(Listener{
                EventType::START_GAME,
                bind(&SoundFacadeFMOD::StartGame, this, placeholders::_1),
                "StartGame"});

            EventManager::GetInstance().SubscribeMulti(Listener{
                EventType::PRESS_P,
                bind(&SoundFacadeFMOD::SoundClaxon, this, placeholders::_1),
                "SoundClaxon"});

            EventManager::GetInstance().SubscribeMulti(Listener(
                EventType::THROW_POWERUP,
                bind(&SoundFacadeFMOD::SoundThrowPowerup, this, placeholders::_1),
                "SoundThrowPowerup"));

            // Quitar
            EventManager::GetInstance().SubscribeMulti(Listener{
                EventType::PRESS_0,
                bind(&SoundFacadeFMOD::StopPrueba, this, placeholders::_1),
                "StopPrueba"});

            EventManager::GetInstance().SubscribeMulti(Listener{
                EventType::HURT,
                bind(&SoundFacadeFMOD::SoundHurt, this, placeholders::_1),
                "SoundHurt"});

            EventManager::GetInstance().SubscribeMulti(Listener{
                EventType::CATCH_TOTEM,
                bind(&SoundFacadeFMOD::SoundCatchTotem, this, placeholders::_1),
                "SoundCatchTotem"});

            EventManager::GetInstance().SubscribeMulti(Listener{
                EventType::CRASH_ENEMY,
                bind(&SoundFacadeFMOD::SoundCrash, this, placeholders::_1),
                "SoundCrash"});

            EventManager::GetInstance().SubscribeMulti(Listener{
                EventType::BREAK_BOX,
                bind(&SoundFacadeFMOD::SoundBreakBox, this, placeholders::_1),
                "SoundBreakBox"});

            EventManager::GetInstance().SubscribeMulti(Listener{
                EventType::DRIFT,
                bind(&SoundFacadeFMOD::SoundBreakBox, this, placeholders::_1),
                "SoundBreakBox"});

            EventManager::GetInstance().SubscribeMulti(Listener{
                EventType::VRANDOM,
                bind(&SoundFacadeFMOD::SoundRandomSentence, this, placeholders::_1),
                "SoundRandomSentence"});

            EventManager::GetInstance().SubscribeMulti(Listener{
                EventType::MENU_OPTION,
                bind(&SoundFacadeFMOD::SoundMenuOption, this, placeholders::_1),
                "SoundMenuOption"});

            // --- STOP

            EventManager::GetInstance().SubscribeMulti(Listener{
                EventType::NO_SHIELD,
                bind(&SoundFacadeFMOD::StopShield, this, placeholders::_1),
                "StopShield"});

            EventManager::GetInstance().SubscribeMulti(Listener{
                EventType::NO_DRIFT,
                bind(&SoundFacadeFMOD::StopDrift, this, placeholders::_1),
                "StopDrift"});

            break;

        case 5:  // EndRace
            break;
        case 7:  // Controls
            break;

        case 8:  // Creadits
            break;

        default:
            break;
    }
}

/**
 * Segun el estado en el que nos encontramos llama a una funcion u otra para cargar bancos.
 * @param numState - Estado en el que nos encontramos.
 */
void SoundFacadeFMOD::LoadSoundByState(const uint8_t numState) {
    switch (numState) {
        case 0:
            break;
        case 1:
            break;
        case 2:  // menu
            UnloadAllBanks();
            LoadSoundBank("Menu", 0);
            break;
        case 3:
            break;
        case 4:  //InGame
            StopEvent("Musica/menu");
            LoadSoundBank("InGame2D", 0);
            LoadSoundBank("InGame3D", 1);
            break;
        case 5:  //EndRace
            StopAllEvents();
            LoadSoundBank("EndRace", 0);
            break;
        case 7:  // Controls
            break;
        case 8:  // Creadits
            break;
        default:
            std::cout << "***** Este estado no existe: " << numState << endl;
    }
}

/*
 * Carga el banco y sus respectivos eventos.
 * @param nameBank - Nombre del banco a cargar.
 * @param type - 1 para eventos 3D y 0 para eventos 2D.
 */
void SoundFacadeFMOD::LoadSoundBank(const string nameBank, const bool type) {
    if (banks.find(nameBank) == banks.end()) {
        banks[nameBank] = nullptr;

        string bank = "./media/fmod/" + nameBank + ".bank";
        ERRCHECK(system->loadBankFile(bank.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &banks[nameBank]));
        cout << "***** Carcado el banco: " << nameBank << endl;
    }

    for (const auto& event : events[nameBank]) {
        LoadSoundEvent(event.c_str(), type);
    }
}

/* Carga los eventos de sonido. Los 3D sin instancias y los 2D con instancia.
 * @param nameEvent - Identificacion del evento en FMOD Studio.
 * @param type - 3D es 1 y 2D es 0
 */
void SoundFacadeFMOD::LoadSoundEvent(const string nameEvent, const bool type) {
    // Si no esta el evento de sonido cargado, lo cargo.
    if (soundDescriptions.find(nameEvent) == soundDescriptions.end()) {
        soundDescriptions[nameEvent] = nullptr;

        string event = "event:/" + nameEvent;
        ERRCHECK(system->getEvent(event.c_str(), &soundDescriptions[nameEvent]));

        if (type) {
            // Se guarda los datos en memoria para que no se vuelvan a cargar cada vez que creo una instancia.
            ERRCHECK(soundDescriptions[nameEvent]->loadSampleData());
        } else {
            eventInstances[nameEvent] = CreateInstance(nameEvent);
        }
    }
}

/**
 * Cambia el valor de un parametro del evento pasado por parametro.
 * @param nameID - Identificador del sonido en el mapa de instancias.
 * @param nameParameter - ID del parametro que quiero cambiar del evento. 
 * @param value - Valor al que quiero cambiar el parametro.
 */
void SoundFacadeFMOD::SetParameter(const string nameID, const string nameParameter, const float value) {
    if (eventInstances.find(nameID) != eventInstances.end()) {
        eventInstances[nameID]->setParameterByName(nameParameter.c_str(), value);
    }
}

/**
 * Se cambia la posicion desde donde se escucha un sonido.
 * TO-DO: Aqui solo se cambia la posicion, para el efecto Doppler hace falta la velocidad. Creo que hay mas cosas a parte.
 */
void SoundFacadeFMOD::SetEventPosition(const string& nameID, const glm::vec3& pos) {
    if (eventInstances.find(nameID) != eventInstances.end()) {
        FMOD_3D_ATTRIBUTES atr;
        atr.position.x = pos.x;
        atr.position.y = pos.y;
        atr.position.z = pos.z;

        ERRCHECK(eventInstances[nameID]->set3DAttributes(&atr));
    }
}

/**
 * Ejecuta el evento de sonido.
 * TO-DO: Actualmente no se puede crear una instancia del mismo evento porque el ID es el mismo.
 * @param nameID - Identificador del sonido en el mapa de instancias.
 */
void SoundFacadeFMOD::PlayEvent(const string nameID) {
    //std::cout << "***** Se ha lanzado el  sonido: " << nameID << endl;
    ERRCHECK(eventInstances[nameID]->start());
}

/**
 * Para todos los sonidos.
 * @param nameID - Identificador del sonido en el mapa de instancias.
 */
void SoundFacadeFMOD::StopAllEvents() {
    for ( auto instance : eventInstances) {
        ERRCHECK(instance.second->stop(FMOD_STUDIO_STOP_IMMEDIATE));
    }
}

/**
 * Para el evento de sonido.
 * @param nameID - Identificador del sonido en el mapa de instancias.
 */
void SoundFacadeFMOD::StopEvent(const string nameID) {
    if (eventInstances.find(nameID) != eventInstances.end() /*&& IsPlaying(eventInstances[nameID])*/) {
        ERRCHECK(eventInstances[nameID]->stop(FMOD_STUDIO_STOP_IMMEDIATE));
    }
}

/**
 * Pone en pause todos los sonidos.
 */
void SoundFacadeFMOD::PauseAllEvent() {
    for (const auto& event : eventInstances) {
        PauseEvent(event.first);
    }
}

/**
 * Reanuda en pause todos los sonidos.
 */
void SoundFacadeFMOD::ResumeAllEvent() {
    for (const auto& event : eventInstances) {
        ResumeEvent(event.first);
    }
}

/**
 * Pone en pause el sonido.
 * @param nameID - Identificador del sonido en el mapa de instancias.
 */
void SoundFacadeFMOD::PauseEvent(const string& nameID) {
    if (eventInstances.find(nameID) != eventInstances.end() && IsPlaying(eventInstances[nameID])) {
        ERRCHECK(eventInstances[nameID]->setPaused(true));
    }
}

/**
 * Reanuda el sonido.
 * @param nameID - Identificador del sonido en el mapa de instancias.
 */
void SoundFacadeFMOD::ResumeEvent(const string& nameID) {
    if (eventInstances.find(nameID) != eventInstances.end() && IsPlaying(eventInstances[nameID])) {
        ERRCHECK(eventInstances[nameID]->setPaused(false));
    }
}

/* Verifica si la instancia de sonido que le enviamos por parametro esta en PLAY o preparandose para el PLAY.
 * @param instance - Instancia del evento a verificar.
 */
bool SoundFacadeFMOD::IsPlaying(FMOD::Studio::EventInstance* instance) {
    FMOD_STUDIO_PLAYBACK_STATE eventState;
    ERRCHECK(instance->getPlaybackState(&eventState));
    if (eventState == FMOD_STUDIO_PLAYBACK_PLAYING || eventState == FMOD_STUDIO_PLAYBACK_STARTING /*|| eventState == FMOD_STUDIO_PLAYBACK_SUSTAINING*/) {
        return true;
    }
    return false;
}

/**
 * Actualiza la fachada de sonido. 
 */
void SoundFacadeFMOD::Update() {
    ERRCHECK(system->update());
}

/*
 ***************************************************************************
 * EVENTOS
 * *************************************************************************
*/

void SoundFacadeFMOD::StartGame(DataMap* d) {
    eventInstances["Coche/motor"] = CreateInstance("Coche/motor");
    PlayEvent("Coche/motor");
    PlayEvent("Ambiente/ambiente");
    PlayEvent("Musica/in_game_1");
    srand(time(NULL));
    character = rand() % 4;
    cout << "++++ Personaje en sonido: " << character << endl;
    SetParameter("Personajes/voces", "Personaje", character);
    SetParameter("Coche/claxon", "Personaje", character);
    SetParameter("Coche/motor", "Personaje", character);
}

void SoundFacadeFMOD::SoundClaxon(DataMap* d) {
    PlayEvent("Coche/claxon");
}

void SoundFacadeFMOD::SoundHurt(DataMap* d) {
    auto mainCharacter = any_cast<bool>((*d)[MAIN_CAR]);

    if (mainCharacter) {
        SetParameter("Personajes/voces", "Tipo", TipoVoz::ChoquePowerup);
        PlayEvent("Personajes/voces");
    }
    eventInstances["Coche/choque_powerup"] = CreateInstance("Coche/choque_powerup");
    PlayEvent("Coche/choque_powerup");
}

void SoundFacadeFMOD::SoundCatchTotem(DataMap* d) {
    eventInstances["Partida/coger_totem"] = CreateInstance("Partida/coger_totem");
    PlayEvent("Partida/coger_totem");
}

// TO-DO: La voz se reproduce si el coche dañado es el principal
void SoundFacadeFMOD::SoundCrash(DataMap* d) {
    bool mainCharacter = any_cast<bool>((*d)[MAIN_CAR]);

    int max = 100;
    int min = 0;
    int randNum = rand() % (max - min + 1) + min;
    if (mainCharacter && randNum <= 33) {
        SetParameter("Personajes/voces", "Tipo", TipoVoz::ChoqueEnemigo);
        PlayEvent("Personajes/voces");
    }
    eventInstances["Coche/choque"] = CreateInstance("Coche/choque");
    if (!IsPlaying(eventInstances["Coche/choque"]))
        PlayEvent("Coche/choque");
}

void SoundFacadeFMOD::SoundBreakBox(DataMap* d) {
    eventInstances["Partida/coger_caja"] = CreateInstance("Partida/coger_caja");
    PlayEvent("Partida/coger_caja");
}

void SoundFacadeFMOD::SoundDrift(DataMap* d) {
    eventInstances["Coche/derrape"] = CreateInstance("Coche/derrape");
    //SetParameter("Personajes/voces", "Tipo", TipoVoz::Derrape);
    PlayEvent("Coche/derrape");
}

void SoundFacadeFMOD::SoundRandomSentence(DataMap* d) {
    SetParameter("Personajes/voces", "Tipo", TipoVoz::Random);
    PlayEvent("Personajes/voces");
}

void SoundFacadeFMOD::SoundMenuOption(DataMap* d) {
    PlayEvent("Menu/cambio_opcion");
}

// TO-DO: Cambiar de eventos 2D a 3D
void SoundFacadeFMOD::SoundThrowPowerup(DataMap* d) {
    typeCPowerUp typepw = any_cast<typeCPowerUp>((*d)[TYPE_POWER_UP]);

    switch (typepw) {
        case typeCPowerUp::RoboJorobo:
            eventInstances["PowerUp/robojorobo"] = CreateInstance("PowerUp/robojorobo");
            PlayEvent("PowerUp/robojorobo");
            break;
        case typeCPowerUp::EscudoMerluzo:
            eventInstances["PowerUp/escudo"] = CreateInstance("PowerUp/escudo");
            PlayEvent("PowerUp/escudo");
            break;
        case typeCPowerUp::SuperMegaNitro:
            // TO-DO: Cambio de parametro del personaje
            break;
        case typeCPowerUp::MelonMolon:
            SetParameter("Personajes/voces", "Tipo", TipoVoz::Powerup);
            PlayEvent("Personajes/voces");
            eventInstances["PowerUp/melonmolon"] = CreateInstance("PowerUp/melonmolon");
            PlayEvent("PowerUp/melonmolon");
            break;
        case typeCPowerUp::TeleBanana:
            SetParameter("Personajes/voces", "Tipo", TipoVoz::Powerup);
            PlayEvent("Personajes/voces");
            eventInstances["PowerUp/telebanana_prov"] = CreateInstance("PowerUp/telebanana_prov");
            PlayEvent("PowerUp/telebanana_prov");
            break;
        case typeCPowerUp::PudinDeFrambuesa:
            SetParameter("Personajes/voces", "Tipo", TipoVoz::Powerup);
            PlayEvent("Personajes/voces");
            eventInstances["PowerUp/pudin"] = CreateInstance("PowerUp/pudin");
            PlayEvent("PowerUp/pudin");
            std::cout << "POWERUP: " << (int)typepw << endl;
            break;
        default:
            cout << "***** El powerup " << (int)typepw << " no tiene sonido" << endl;
    }
}

// -------------> STOP

void SoundFacadeFMOD::StopPrueba(DataMap* d) {
    StopEvent("Personajes/voces");
}

void SoundFacadeFMOD::StopShield(DataMap* d) {
    StopEvent("PowerUp/escudo");
    eventInstances["PowerUp/escudo_roto"] = CreateInstance("PowerUp/escudo_roto");
    PlayEvent("PowerUp/escudo_roto");
}

void SoundFacadeFMOD::StopDrift(DataMap* d) {
    StopEvent("Coche/derrape");
}
