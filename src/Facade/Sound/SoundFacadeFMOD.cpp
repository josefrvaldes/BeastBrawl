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
        std::cerr << file << "(Linea: " << line << "): Se ha producido un error de FMOD " << result << " - " << FMOD_ErrorString(result) << std::endl;
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

//Puntero a funcion
void SoundFacadeFMOD::SonarClaxon(Data d) {
    SonarUnaVez("pedo");
}
/*void SoundFacadeFMOD::SonidoPedo(Data d) {
    SonarUnaVez("pedo");
}
void SoundFacadeFMOD::SonidoClaxon(Data d) {
    SonarUnaVez("claxon");
}
void SoundFacadeFMOD::SonidoPedo(Data d) {
    SonarUnaVez("pedo");
}*/

//Puntero a funcion
void SoundFacadeFMOD::SonarUnaVez(string s) {
    cout << "Se ha lanzado el evento PruebaSonido" << endl;

    auto description = GetDescriptions();
    auto instances = GetInstances();

    if (description.find("choque_enemigo") != description.end() /*&& instances.find("choque_enemigo") == instances.end()*/) {
        FMOD::Studio::EventInstance* instance = nullptr;
        ERRCHECK(description["choque_enemigo"]->createInstance(&instance));
        ERRCHECK(instance->start());
        //ERRCHECK( instance->release() );
        InsertInstance("choque_enemigo", instance);
    }
    //else if (instances.find("choque_enemigo") != instances.end()) {
    //
    //}
}

void SoundFacadeFMOD::InitSoundEngine() {
    ERRCHECK(FMOD::Studio::System::create(&system));

    ERRCHECK(system->getCoreSystem(&coreSystem));
    ERRCHECK(coreSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0));

    //Inicializa FMOD Studio, inicializando tambien FMOD Core.
    ERRCHECK(system->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0));

    LoadMasterBank();
}

/*void SoundFacadeFMOD::InitializeState(int state) {
    LoadBanks(state);
    switch (state)
    {
    case 0:
        break;
    
    default:
        break;
    }
}*/

void SoundFacadeFMOD::SubscribeToGameEvents(int state) {
    switch (state) {
        case 1:  // menu
            // sonidito cambiar opcion al pulsar tecla

            // musiquita de fondo

            break;

        case 2:  // InGame
            // claxon
            EventManager::GetInstance()->Suscribe(Listener{
                EventType::PRESS_1,
                bind(&SoundFacadeFMOD::SonarClaxon, this, placeholders::_1),
                "pruebaSonido"});

            /*EventManager::GetInstance()->SuscribeMulti(Listener(
                EventType::PRESS_A,
                bind(&ManCar::TurnLeftCar, this, placeholders::_1),
                "TurnLeftCar"));*/

            // motor

            // musicaGanar

            // musicaPerder
            break;

        case 3:  // ventana elegir personaje
            // cada vez que selecciono un personaje, dice una frase ->
            // frasecita de personaje al ser elegido

            // musiquita de fondo

            break;

        default:
            break;
    }
}

void SoundFacadeFMOD::TerminateSoundEngine() {
    soundDescriptions.clear();

    //Recorre el mapa de bancos liberandolos.
    unordered_map<string, FMOD::Studio::Bank*>::iterator it = banks.begin();
    while (it != banks.end()) {
        it->second->unload();
        it++;
    }
    banks.clear();

    UnloadMasterBank();

    ERRCHECK(system->release());
}

//TO-DO: Quitar el dichoso Common_MediaPath.
void SoundFacadeFMOD::LoadMasterBank() {
    ERRCHECK(system->loadBankFile("./media/fmod/Master.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank));
    ERRCHECK(system->loadBankFile("./media/fmod/Master.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank));
}

void SoundFacadeFMOD::UnloadMasterBank() {
    ERRCHECK(stringsBank->unload());
    ERRCHECK(masterBank->unload());
}

void SoundFacadeFMOD::SetState(const uint16_t numState) {
    // TODO: Liberar bancos de estados anteriores¿?
    LoadBanks(numState);
    SubscribeToGameEvents(numState);
}

void SoundFacadeFMOD::LoadBanks(const uint16_t numBank) {
    //TO-DO: Esto se puede optimizar
    switch (numBank) {
        case 0:
            //LoadBankMenu();
            break;
        case 1:
            //Load...
            break;
        case 2:
            LoadBanksInGame();
            break;
        default:
            cout << "Este banco no existe: " << numBank << endl;
    }
}

void SoundFacadeFMOD::LoadBanksInGame() {
    if (banks.find("InGame") == banks.end()) {
        banks["InGame"] = nullptr;
        ERRCHECK(system->loadBankFile("./media/fmod/InGame.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &banks["InGame"]));
    }
    cout << "***** Voy a intentar cargar los eventos" << endl;
    LoadEvent("choque_enemigo");
    LoadEvent("lanzo_powerup");
}

//Carga eventos de sonido que pueden sonar muchas veces durante la partida
void SoundFacadeFMOD::LoadEvent(const char* nameEvent) {
    //Si no esta el evento de sonido cargado, lo cargo.
    FMOD::Studio::EventDescription* description = NULL;
    if (soundDescriptions.find(nameEvent) == soundDescriptions.end()) {
        soundDescriptions[nameEvent] = nullptr;

        //TO-DO: Mejorar esta concatenacion
        char* charEvent = "event:/";
        char* event = (char*)malloc(1 + strlen(charEvent) + strlen(nameEvent));
        strcpy(event, charEvent);
        strcat(event, nameEvent);

        //Cargo el evento
        ERRCHECK(system->getEvent(event, &soundDescriptions[nameEvent]));

        //Se guarda los datos en memoria para que no se vuelvan a cargar cada vez.
        ERRCHECK(soundDescriptions[nameEvent]->loadSampleData());
    }
}

void SoundFacadeFMOD::InsertInstance(const char* nameEvent, FMOD::Studio::EventInstance* instance) {
    eventInstances[nameEvent] = instance;
}

void SoundFacadeFMOD::UnloadBank(const char*) {
}

void SoundFacadeFMOD::Update() {
    FMOD_STUDIO_PLAYBACK_STATE eventState;
    for (auto event : eventInstances) {
        ERRCHECK(event.second->getPlaybackState(&eventState));
        if (eventState != FMOD_STUDIO_PLAYBACK_PLAYING || eventState != FMOD_STUDIO_PLAYBACK_STARTING) {
            //event.second->release();
            //delete event.second;
            //eventInstances.erase(event.first);
        }
    }

    ERRCHECK(system->update());
}