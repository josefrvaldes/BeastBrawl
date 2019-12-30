#include <string.h>
#include <iostream>

#include "SoundFacadeFMOD.h"
#include "SoundFacadeManager.h"

using namespace std;

/*
 * FMOD ERRORS
 */

#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

void ERRCHECK_fn(FMOD_RESULT result, const char* file, int line) {
    if (result != FMOD_OK) {
        cerr << file << " (Linea: " << line << "): FMOD HA CASCAO" << " -> " << FMOD_ErrorString(result) << " Codigo de error: " << result << endl;
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
    cout << "***** Sound engine ON" << endl;
}

/*
 * Libera los audios y bancos de sonido.
 */
void SoundFacadeFMOD::TerminateSoundEngine() {

    UnloadAllBanks();
    UnloadMasterBank();

    ERRCHECK(system->release());
    cout << "***** Sound engine K.O." << endl;
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
    cout << "***** Master bank a la shit" << endl;
}

/*
 * Libera los sonidos que se han cargado en memoria y los bancos.
 */
void SoundFacadeFMOD::UnloadAllBanks() {
    for(auto instance: eventInstances) {
        ERRCHECK(instance.second->release());
    }
    eventInstances.clear();
    soundDescriptions.clear();

    for(auto bank: banks) {
        ERRCHECK(bank.second->unload());
    }
    banks.clear();
    cout << "***** Bancos y sonidos liberados" << endl;

}


/*
 * Libera los bancos que habian cargados anteriormente, si habian.
 * Carga los nuevos bancos y se suscribe a los nuevos eventos.
 * @param numState - Estado en el que nos encontramos.
 */
void SoundFacadeFMOD::SetState(const uint8_t numState) {
    UnloadAllBanks();
    LoadSoundByState(numState);
    SubscribeToGameEvents(numState);
    cout << "***** Bancos cargados" << endl;
}

/* 
 * Se suscribe a los eventos segun el estado en el que se encuentra el juego.
 * @param numState - Estado en el que nos encontramos.
 */
void SoundFacadeFMOD::SubscribeToGameEvents(const uint8_t numState) {
    switch (numState) {
        case 1:     // Intro 
            break;
        case 2:     // Menu
            break;
        case 3:     // Map
            break;
        case 4:     // InGame
            EventManager::GetInstance()->SuscribeMulti(Listener{
                EventType::PRESS_P,
                bind(&SoundFacadeFMOD::SoundClaxon, this, placeholders::_1),
                "SoundClaxon"});

            EventManager::GetInstance()->SuscribeMulti(Listener(
                EventType::THROW_POWERUP,
                bind(&SoundFacadeFMOD::ThrowPowerup, this, placeholders::_1),
                "ThrowPowerup"));

            EventManager::GetInstance()->SuscribeMulti(Listener{
                EventType::PRESS_0,
                bind(&SoundFacadeFMOD::StopPrueba, this, placeholders::_1),
                "StopPrueba"});

            break;

        case 5:     //EndRace
            break;

        case 7:     // Controls
            break;

        case 8:     // Creadits
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
        case 2: // menu
            break;
        case 3:
            break;
        case 4: //InGame
            LoadSoundBank("InGame2D", 0);
            LoadSoundBank("InGame3D", 1);
            break;
        case 5:     //EndRace
            LoadSoundBank("InGameMusic", 0);    //TO-DO: Cambiar esto de sitio
            break;
        case 7:     // Controls
            break;
        case 8:     // Creadits
            break;
        default:
            cout << "***** Este estado no existe: " << numState << endl;
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
    }

    for (auto event: events[nameBank]) {
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
            // Crea la instancia para los eventos de sonido 2D.
            FMOD::Studio::EventInstance* instance = nullptr;
            ERRCHECK(soundDescriptions[nameEvent]->createInstance(&instance));
            eventInstances[nameEvent] = instance;
        }
    }
}


/**
 * Ejecuta el evento de sonido 3D creando primero una instancia y guardandola.
 * TO-DO: Actualmente no se puede crear una instancia del mismo evento porque el ID es el mismo.
 * @param nameID - Identificador del sonido en el mapa de instancias.
 */
void SoundFacadeFMOD::PlayEvent3D(const string nameID) {
    cout << "***** Se ha lanzado el evento: " << nameID << endl;

    if (soundDescriptions.find(nameID) != soundDescriptions.end()) {
        FMOD::Studio::EventInstance* instance = nullptr;
        ERRCHECK(soundDescriptions[nameID]->createInstance(&instance));
        ERRCHECK(instance->start());
        eventInstances[nameID] = instance;
    }
}

/**
 * Ejecuta el evento de sonido 2D.
 * @param nameID - Identificador del sonido en el mapa de instancias.
 */
void SoundFacadeFMOD::PlayEvent2D(const string nameID) {
    cout << "***** Se ha lanzado el evento: " << nameID << endl;

    if (soundDescriptions.find(nameID) != soundDescriptions.end() 
        && eventInstances.find(nameID) != eventInstances.end() 
        && !IsPlaying(eventInstances[nameID])
    ) {
        ERRCHECK(eventInstances[nameID]->start());
    }
}

/**
 * Para el evento de sonido.
 * @param nameID - Identificador del sonido en el mapa de instancias.
 */
void SoundFacadeFMOD::StopEvent(const string nameID) {
    if (eventInstances.find(nameID) != eventInstances.end() && IsPlaying(eventInstances[nameID])) {
        ERRCHECK(eventInstances[nameID]->stop(FMOD_STUDIO_STOP_IMMEDIATE));
    }
}

/**
 * Pone en pause todos los sonidos.
 */
void SoundFacadeFMOD::PauseAllEvent() {
    for (auto event : eventInstances) {
        PauseEvent(event.first);
    }
}

/**
 * Reanuda en pause todos los sonidos.
 */
void SoundFacadeFMOD::ResumeAllEvent() {
    for (auto event : eventInstances) {
        ResumeEvent(event.first);
    }
}

/**
 * Pone en pause el sonido.
 * @param nameID - Identificador del sonido en el mapa de instancias.
 */
void SoundFacadeFMOD::PauseEvent(const string nameID) {
    if (eventInstances.find(nameID) != eventInstances.end() && IsPlaying(eventInstances[nameID])) {
        ERRCHECK(eventInstances[nameID]->setPaused(true));
    }
}

/**
 * Reanuda el sonido.
 * @param nameID - Identificador del sonido en el mapa de instancias.
 */
void SoundFacadeFMOD::ResumeEvent(const string nameID) {
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
 *
 * EVENTOS
 * 
*/

void SoundFacadeFMOD::SoundClaxon(DataMap d) {
    PlayEvent2D("Coche/claxon");
}

void SoundFacadeFMOD::ThrowPowerup(DataMap d) {
    typeCPowerUp typepw = any_cast<typeCPowerUp>(d["typePowerUp"]);

    switch ((int)typepw) {
        case 1:     // Robojorobo
            // TO-DO: ¿Cambiar a 3D?
            PlayEvent2D("PowerUp/robojorobo");
            break;
        case 2:     // Nitro
            break;
        case 4:     // Escudomerluzo
            break;
        default:    // Otro
            PlayEvent2D("Personajes/powerup");
            break;
    }
}


void SoundFacadeFMOD::StopPrueba(DataMap d) {
    StopEvent("Personajes/powerup");
}

