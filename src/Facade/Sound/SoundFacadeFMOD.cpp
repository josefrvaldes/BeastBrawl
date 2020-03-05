#include <cstdlib>
#include <string>
#include <iostream>

#include "SoundFacadeFMOD.h"

using namespace std;

/*
 * SOUND FACADE FMOD
 */

SoundFacadeFMOD::~SoundFacadeFMOD() {
    delete soundEngine;
}

void SoundFacadeFMOD::Initialize() {
    soundEngine = new CLSE::SoundEngine();
}

/**
 * Libera los audios y bancos de sonido.
 */
void SoundFacadeFMOD::Terminate() {
    soundEngine->TerminateSoundEngine();
}


/*
 ************************************************
 * NODOS
 ************************************************
 */

/**
 * Crea la instancia de sonido 2D
 */
void SoundFacadeFMOD::CreateSound2D(const string& nameEvent) {
    soundEngine->CreateSoundNode2D(nameEvent);
}

/**
 * Crea la instancia de sonido 2D
 */
void SoundFacadeFMOD::CreateSoundEstatic3D(uint16_t idE, glm::vec3& pos, string& nameEvent) {
    soundEngine->CreateSoundNodeEstatic3D(idE, pos, nameEvent);
}

/**
 * Crea la instancia de sonido 2D
 */
void SoundFacadeFMOD::CreateSoundDinamic3D(uint16_t idE, glm::vec3& pos, string& nameEvent) {
    soundEngine->CreateSoundNodeDinamic3D(idE, pos, nameEvent);
}



/*
 ************************************************
 * ESTADOS
 ************************************************
 */


/**
 * Libera los bancos que habian cargados anteriormente, si habian.
 * Carga los nuevos bancos y se suscribe a los nuevos eventos.
 * @param numState - Estado en el que nos encontramos.
 */
void SoundFacadeFMOD::SetState(const uint8_t numState) {
    LoadSoundByState(numState);
    SubscribeToGameEvents(numState);
}

/**
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

        case 8:  // Credits
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
            soundEngine->UnloadAllBanks();
            LoadSoundBank("Menu", 0);
            break;
        case 3:
            break;
        case 4:  //InGame
            StopEvent("Musica/menu");
            LoadSoundBank("InGame2D", 0);
            LoadSoundBank("InGame3DE", 1);
            LoadSoundBank("InGame3DD", 1);
            PlayEvent("Ambiente/ambiente");
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

/**
 * Carga el banco y sus respectivos eventos.
 * @param nameBank - Nombre del banco a cargar.
 * @param type - 1 para eventos 3D y 0 para eventos 2D.
 */
void SoundFacadeFMOD::LoadSoundBank(const string& nameBank, const bool type) {
    cout << "********* Voy a cargar el banco: " << nameBank << endl;
    soundEngine->LoadSoundBank(nameBank);
    auto it = events.find(nameBank);
    if ( it != events.end()) {
        for (const auto& event : events[nameBank]) {
            LoadSoundEvent(event, type);
        }
    }

}

/**
 * Carga los eventos de sonido. Los 3D sin instancias y los 2D con instancia.
 * @param nameEvent - Identificacion del evento en FMOD Studio.
 * @param type - 3D es 1 y 2D es 0
 */
void SoundFacadeFMOD::LoadSoundEvent(const string& nameEvent, const bool type) {
    soundEngine->LoadSoundEvent(nameEvent, type);
}

/**
 * Cambia el valor de un parametro del evento pasado por parametro.
 * @param nameID - Identificador del sonido en el mapa de instancias.
 * @param nameParameter - ID del parametro que quiero cambiar del evento. 
 * @param value - Valor al que quiero cambiar el parametro.
 */
void SoundFacadeFMOD::SetParameter(const string& nameID, const string& nameParameter, const float value) {
    soundEngine->SetParameter(nameID, nameParameter, value);
}

/**
 * Se cambia la posicion desde donde se escucha un sonido.
 * TO-DO: Aqui solo se cambia la posicion, para el efecto Doppler hace falta la velocidad. Creo que hay mas cosas a parte.
 */
void SoundFacadeFMOD::SetEventPositionEstatic3D(const string& nameID, const glm::vec3& pos) {
    soundEngine->Set3DAttributes(nameID, pos);
}

/**
 * Se cambia la posicion desde donde se escucha un sonido.
 * TO-DO: Aqui solo se cambia la posicion, para el efecto Doppler hace falta la velocidad. Creo que hay mas cosas a parte.
 */
void SoundFacadeFMOD::SetEventPositionDinamic3D(const string& nameID, const glm::vec3& pos) {
    soundEngine->Set3DAttributes(nameID, pos);
}

/**
 * Ejecuta el evento de sonido.
 * TO-DO: Actualmente no se puede crear una instancia del mismo evento porque el ID es el mismo.
 * @param nameID - Identificador del sonido en el mapa de instancias.
 */
void SoundFacadeFMOD::PlayEvent(const string& nameID) {
    soundEngine->PlayEvent(nameID);
}

/**
 * Para todos los sonidos.
 * @param nameID - Identificador del sonido en el mapa de instancias.
 */
void SoundFacadeFMOD::StopAllEvents() {
    soundEngine->StopAllEvents();
}

/**
 * Para el evento de sonido.
 * @param nameID - Identificador del sonido en el mapa de instancias.
 */
void SoundFacadeFMOD::StopEvent(const string& nameID) {
    soundEngine->StopEvent(nameID);
}

/**
 * Pone en pause todos los sonidos.
 */
void SoundFacadeFMOD::PauseAllEvent() {
    soundEngine->PauseAllEvents();
}

/**
 * Reanuda en pause todos los sonidos.
 */
void SoundFacadeFMOD::ResumeAllEvent() {
    soundEngine->ResumeAllEvents();
}

/**
 * Pone en pause el sonido.
 * @param nameID - Identificador del sonido en el mapa de instancias.
 */
void SoundFacadeFMOD::PauseEvent(const string& nameID) {
    soundEngine->PauseEvent(nameID);
}

/**
 * Reanuda el sonido.
 * @param nameID - Identificador del sonido en el mapa de instancias.
 */
void SoundFacadeFMOD::ResumeEvent(const string& nameID) {
    soundEngine->ResumeEvent(nameID);
}


/**
 *
 */
void SoundFacadeFMOD::UpdateCars(const vector<shared_ptr<Entity> > &e) {

}

/**
 *
 */
 void SoundFacadeFMOD::UpdatePowerUps(const vector<shared_ptr<Entity> > &) {

 }

 /**
  *
  */
 void SoundFacadeFMOD::UpdateTotem(const vector<shared_ptr<Entity> > &) {

 }

 /**
  *
  */
 void SoundFacadeFMOD::UpdateListener(const shared_ptr<CarHuman> &mainCar) {
    //cout << "############# UPDATE LISTENER" << endl;
    auto pos = static_cast<CTransformable*>(mainCar->GetComponent(CompType::TransformableComp).get());
    //cout << "LA POSICION DEL PUTO COCHE ES: " << pos->position.x << " " << pos->position.y << " " << pos->position.z << endl;
    if(pos) {
        soundEngine->SetListenerPosition(pos->position);

    }
 }

/**
 * Actualiza la fachada de sonido. 
 */
void SoundFacadeFMOD::Update() {
    soundEngine->UpdateEngine();
}

/*
 ***************************************************************************
 * EVENTOS
 * *************************************************************************
*/

void SoundFacadeFMOD::StartGame(DataMap* d) {
    /*auto pos = glm::vec3(0.0f,0.0f,0.0f);
    string name = "Coche/motor";
    CreateSoundDinamic3D(0, pos, name);
    PlayEvent("Coche/motor0");*/
    //PlayEvent("Ambiente/ambiente");
    //PlayEvent("Musica/in_game_1");
    srand(time(nullptr));
    character = rand() % 5;
    cout << "++++ Personaje en sonido: " << character << endl;
    SetParameter("Personajes/voces", "Personaje", character);
    SetParameter("Coche/claxon", "Personaje", character);
    //SetParameter("Coche/motor", "Personaje", character);
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

    auto pos = glm::vec3(0.0f,0.0f,0.0f);
    string name = "Coche/choque_powerup";
    CreateSoundEstatic3D(0, pos, name);
    PlayEvent(name);
}

void SoundFacadeFMOD::SoundCatchTotem(DataMap* d) {
    auto pos = glm::vec3(0.0f,0.0f,0.0f);
    string name = "Partida/coger_totem";
    CreateSoundEstatic3D(0, pos, name);
    PlayEvent("Partida/coger_totem0");
}

// TO-DO: La voz se reproduce si el coche dañado es el principal
// TO-DO: NO SE LLAMA NUNCA A ESTE EVENTO
void SoundFacadeFMOD::SoundCrash(DataMap* d) {
    bool mainCharacter = any_cast<bool>((*d)[MAIN_CAR]);

    int max = 100;
    int min = 0;
    int randNum = rand() % (max - min + 1) + min;
    if (mainCharacter && randNum <= 33) {
        SetParameter("Personajes/voces", "Tipo", TipoVoz::ChoqueEnemigo);
        PlayEvent("Personajes/voces");
    }
    /*eventInstances3DD["Coche/choque"] = CreateInstance("Coche/choque");
    if (!IsPlaying(*eventInstances3DD.find("Coche/choque")->second)) {
        PlayEvent("Coche/choque");
    }*/
}

void SoundFacadeFMOD::SoundBreakBox(DataMap* d) {
    auto idEntity = any_cast<uint16_t>((*d)[ID]);
    string mapID = "Partida/coger_caja" + to_string(idEntity);
    /*auto pos = glm::vec3(0.0f,0.0f,0.0f);
    string name = "Partida/coger_caja";
    CreateSoundEstatic3D(0, pos, name);*/

    PlayEvent(mapID);
}

void SoundFacadeFMOD::SoundDrift(DataMap* d) {
    /*eventInstances3DD["Coche/derrape"] = CreateInstance("Coche/derrape");
    //SetParameter("Personajes/voces", "Tipo", TipoVoz::Derrape);
    PlayEvent("Coche/derrape");*/
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
    auto typepw = any_cast<typeCPowerUp>((*d)[TYPE_POWER_UP]);

    auto pos = glm::vec3(0.0f,0.0f,0.0f);
    string name;

    switch (typepw) {
        case typeCPowerUp::RoboJorobo:
            name = "PowerUp/robojorobo";
            CreateSoundDinamic3D(0, pos, name);
            PlayEvent("PowerUp/robojorobo0");
            break;
        case typeCPowerUp::EscudoMerluzo:
            name = "PowerUp/escudo";
            CreateSoundDinamic3D(0, pos, name);
            PlayEvent("PowerUp/escudo0");
            break;
        case typeCPowerUp::SuperMegaNitro:
            // TO-DO: Cambio de parametro del personaje
            break;
        case typeCPowerUp::MelonMolon:
            SetParameter("Personajes/voces", "Tipo", TipoVoz::Powerup);
            PlayEvent("Personajes/voces");
            name = "PowerUp/melonmolon";
            CreateSoundDinamic3D(0, pos, name);
            PlayEvent("PowerUp/melonmolon0");
            break;
        case typeCPowerUp::TeleBanana:
            SetParameter("Personajes/voces", "Tipo", TipoVoz::Powerup);
            PlayEvent("Personajes/voces");
            name = "PowerUp/telebanana_prov";
            CreateSoundDinamic3D(0, pos, name);
            PlayEvent("PowerUp/telebanana_prov0");
            break;
        case typeCPowerUp::PudinDeFrambuesa:
            SetParameter("Personajes/voces", "Tipo", TipoVoz::Powerup);
            PlayEvent("Personajes/voces");
            name = "PowerUp/melonmolon";
            CreateSoundDinamic3D(0, pos, name);
            PlayEvent("PowerUp/pudin0");
            std::cout << "POWERUP: " << (int)typepw << endl;
            break;
        default:
            cout << "***** El powerup " << (int)typepw << " no tiene sonido" << endl;
    }
}

// -------------> STOP


void SoundFacadeFMOD::StopShield(DataMap* d) {
    StopEvent("PowerUp/escudo0");

    auto pos = glm::vec3(0.0f,0.0f,0.0f);
    string name = "PowerUp/escudo_roto";
    CreateSoundEstatic3D(0, pos, name);
    PlayEvent("PowerUp/escudo_roto0");
}

void SoundFacadeFMOD::StopDrift(DataMap* d) {
    StopEvent("Coche/derrape");
}