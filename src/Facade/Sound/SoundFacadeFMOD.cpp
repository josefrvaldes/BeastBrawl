#include <cstdlib>
#include <string>
#include <iostream>

#include "SoundFacadeFMOD.h"
#include <Components/CId.h>
#include <Components/CTransformable.h>
#include <Components/CCar.h>
#include <GameValues.h>

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
void SoundFacadeFMOD::CreateSoundEstatic3D(uint16_t idE, glm::vec3& pos, string& nameEvent, bool p) {
    soundEngine->CreateSoundNodeEstatic3D(idE, pos, nameEvent, p);
}

/**
 * Crea la instancia de sonido 2D
 */
void SoundFacadeFMOD::CreateSoundDinamic3D(uint16_t idE, glm::vec3& pos, string& nameEvent, bool p, bool c) {
    soundEngine->CreateSoundNodeDinamic3D(idE, pos, nameEvent, p, c);
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
        case 0:         // INTRO            
            EventManager::GetInstance().SubscribeMulti(Listener{
                    EventType::MENU_OK,
                    bind(&SoundFacadeFMOD::SoundMenuOk, this, placeholders::_1),
                    "SoundMenuOk"});
            break;
        case 1:         // MENU
            EventManager::GetInstance().SubscribeMulti(Listener{
                    EventType::MENU_OPTION,
                    bind(&SoundFacadeFMOD::SoundMenuOption, this, placeholders::_1),
                    "SoundMenuOption"});
            
            EventManager::GetInstance().SubscribeMulti(Listener{
                    EventType::MENU_OK,
                    bind(&SoundFacadeFMOD::SoundMenuOk, this, placeholders::_1),
                    "SoundMenuOk"});
            
            EventManager::GetInstance().SubscribeMulti(Listener{
                    EventType::MENU_BACK,
                    bind(&SoundFacadeFMOD::SoundMenuBack, this, placeholders::_1),
                    "SoundMenuBack"});
            break;
        case 2:         // SELECCION PERSONAJE
            EventManager::GetInstance().SubscribeMulti(Listener{
                    EventType::VRANDOM,
                    bind(&SoundFacadeFMOD::SoundRandomSentence, this, placeholders::_1),
                    "SoundRandomSentence"});

            EventManager::GetInstance().SubscribeMulti(Listener{
                    EventType::UPDATE_SOUNDCHARACTER,
                    bind(&SoundFacadeFMOD::SetCharacter, this, placeholders::_1),
                    "SetCharacter"});

            EventManager::GetInstance().SubscribeMulti(Listener{
                    EventType::MENU_OPTION,
                    bind(&SoundFacadeFMOD::SoundMenuOption, this, placeholders::_1),
                    "SoundMenuOption"});
            
            EventManager::GetInstance().SubscribeMulti(Listener{
                    EventType::MENU_OK,
                    bind(&SoundFacadeFMOD::SoundMenuOk, this, placeholders::_1),
                    "SoundMenuOk"});
            
            EventManager::GetInstance().SubscribeMulti(Listener{
                    EventType::MENU_BACK,
                    bind(&SoundFacadeFMOD::SoundMenuBack, this, placeholders::_1),
                    "SoundMenuBack"});
            break;
        case 3:         // OPCIONES DE PARTIDA
            EventManager::GetInstance().SubscribeMulti(Listener{
                    EventType::MENU_OPTION,
                    bind(&SoundFacadeFMOD::SoundMenuOption, this, placeholders::_1),
                    "SoundMenuOption"});
            
            EventManager::GetInstance().SubscribeMulti(Listener{
                    EventType::MENU_OK,
                    bind(&SoundFacadeFMOD::SoundMenuOk, this, placeholders::_1),
                    "SoundMenuOk"});
            
            EventManager::GetInstance().SubscribeMulti(Listener{
                    EventType::MENU_BACK,
                    bind(&SoundFacadeFMOD::SoundMenuBack, this, placeholders::_1),
                    "SoundMenuBack"});
            break;
        case 4: {       // INGAME

            EventManager::GetInstance().SubscribeMulti(Listener{
                    EventType::PRESS_P,
                    bind(&SoundFacadeFMOD::SoundClaxon, this, placeholders::_1),
                    "SoundClaxon"});

            EventManager::GetInstance().SubscribeMulti(Listener{
                    EventType::CLOCK,
                    bind(&SoundFacadeFMOD::SoundClock, this, placeholders::_1),
                    "SoundClock"});

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
                    EventType::CRASH,
                    bind(&SoundFacadeFMOD::SoundCrash, this, placeholders::_1),
                    "SoundCrash"});

            EventManager::GetInstance().SubscribeMulti(Listener{
                    EventType::CRASH_WALL,
                    bind(&SoundFacadeFMOD::SoundCrashWall, this, placeholders::_1),
                    "SoundCrashWall"});

            EventManager::GetInstance().SubscribeMulti(Listener{
                    EventType::BREAK_BOX,
                    bind(&SoundFacadeFMOD::SoundBreakBox, this, placeholders::_1),
                    "SoundBreakBox"});

            EventManager::GetInstance().SubscribeMulti(Listener{
                    EventType::DRIFT,
                    bind(&SoundFacadeFMOD::SoundBreakBox, this, placeholders::_1),
                    "SoundBreakBox"});


            // --- STOP

            EventManager::GetInstance().SubscribeMulti(Listener{
                    EventType::NO_SHIELD,
                    bind(&SoundFacadeFMOD::StopShield, this, placeholders::_1),
                    "StopShield"});

            EventManager::GetInstance().SubscribeMulti(Listener{
                    EventType::NO_DRIFT,
                    bind(&SoundFacadeFMOD::StopDrift, this, placeholders::_1),
                    "StopDrift"});

            EventManager::GetInstance().SubscribeMulti(Listener{
                    EventType::NOT_CLOCK,
                    bind(&SoundFacadeFMOD::StopClock, this, placeholders::_1),
                    "StopClock"});

            EventManager::GetInstance().SubscribeMulti(Listener{
                    EventType::STOP_SOUND_MM,
                    bind(&SoundFacadeFMOD::StopSoundMM, this, placeholders::_1),
                    "StopSoundMM"});

            EventManager::GetInstance().SubscribeMulti(Listener{
                    EventType::STOP_SOUND_TB,
                    bind(&SoundFacadeFMOD::StopSoundTB, this, placeholders::_1),
                    "StopSoundTB"});

            break;
        }
        case 5:         // ENDRACE
            EventManager::GetInstance().SubscribeMulti(Listener{
                    EventType::MENU_OPTION,
                    bind(&SoundFacadeFMOD::SoundMenuOption, this, placeholders::_1),
                    "SoundMenuOption"});
            
            EventManager::GetInstance().SubscribeMulti(Listener{
                    EventType::MENU_OK,
                    bind(&SoundFacadeFMOD::SoundMenuOk, this, placeholders::_1),
                    "SoundMenuOk"});

            EventManager::GetInstance().SubscribeMulti(Listener{
                    EventType::VICTORY,
                    bind(&SoundFacadeFMOD::SoundVictory, this, placeholders::_1),
                    "SoundVictory"});

            EventManager::GetInstance().SubscribeMulti(Listener{
                    EventType::DEFEAT,
                    bind(&SoundFacadeFMOD::SoundDefeat, this, placeholders::_1),
                    "SoundDefeat"});
            break;
        case 6:         // LOBBY
            EventManager::GetInstance().SubscribeMulti(Listener{
                    EventType::MENU_BACK,
                    bind(&SoundFacadeFMOD::SoundMenuBack, this, placeholders::_1),
                    "SoundMenuBack"});
            break;
        /*case 7:         // PAUSE - Noy hay eventmanager
            break;*/
        case 8:         // SETTINGS
            EventManager::GetInstance().SubscribeMulti(Listener{
                    EventType::MENU_OPTION,
                    bind(&SoundFacadeFMOD::SoundMenuOption, this, placeholders::_1),
                    "SoundMenuOption"});
            
            EventManager::GetInstance().SubscribeMulti(Listener{
                    EventType::MENU_BACK,
                    bind(&SoundFacadeFMOD::SoundMenuBack, this, placeholders::_1),
                    "SoundMenuBack"});

            EventManager::GetInstance().SubscribeMulti(Listener{
                    EventType::SET_GLOBAL_VOLUME,
                    bind(&SoundFacadeFMOD::SetGlobalVolume, this, placeholders::_1),
                    "SetGlobalVolume"});
            break;
        case 9:         // CREDITS
            EventManager::GetInstance().SubscribeMulti(Listener{
                    EventType::MENU_BACK,
                    bind(&SoundFacadeFMOD::SoundMenuBack, this, placeholders::_1),
                    "SoundMenuBack"});
            break;
        case 10:        // CONTROLS
            EventManager::GetInstance().SubscribeMulti(Listener{
                    EventType::MENU_BACK,
                    bind(&SoundFacadeFMOD::SoundMenuBack, this, placeholders::_1),
                    "SoundMenuBack"});
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
        case 0:         // INTRO
            //soundEngine->UnloadAllBanks();
            //StopEvent("Musica/menu");
            LoadSoundBank("Menu", 0);
            if (!soundEngine->IsPlaying2D("Musica/menu")) {
                PlayEvent("Musica/menu");
            }
            break;
        case 1:         // MENU
            StopEvent("Musica/fin_partida");
            if(!soundEngine->IsPlaying2D("Musica/menu")) {
                ResumeAllEvent();
                StopAllEvents();            //Esto me esta jodiendo el aceptar del PAUSE y ENDRACE. Se arreglaria dandole un tiempecito al volver.
                PlayEvent("Musica/menu");
            }
            break;
        case 2:         // SELECCION PERSONAJE
            StopEvent("Musica/fin_partida");
            if(!soundEngine->IsPlaying2D("Musica/menu")) {
                PlayEvent("Musica/menu");
            }
            LoadSoundBank("InGame2D", 0);
            break;
        case 3:       // OPCIONES DE PARTIDA
            StopEvent("Personajes/voces");
            break;
        case 4:         // INGAME
            StopEvent("Musica/menu");
            StopEvent("Musica/fin_partida");
            LoadSoundBank("InGame3DE", 1);
            LoadSoundBank("InGame3DD", 1);
            StartGame();
            break;
        case 5:         // ENDRACE
            StopAllEvents();
            LoadSoundBank("EndRace", 0);
            PlayEvent("Musica/fin_partida");
            SoundVictoryVoice();
            break;
        /*case 6:       // LOBBY
            break;*/
        case 7:         // PAUSE
            PauseAllEvent();
            ResumeEvent("Menu/cambio_opcion");
            ResumeEvent("Menu/atras");
            ResumeEvent("Menu/aceptar");
            break;
        /*case 8:       // SETTINGS
            break;*/
        /*case 9:       // CREDITS
            break;*/
        /*case 10:      // CONTROLS
            break;*/
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
    //cout << "********* Voy a cargar el banco: " << nameBank << endl;
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
    //cout << "VOY A CAMBIAR EL PARAMETRO " << nameParameter << " DE " << nameID << " A VALOR " << value << endl; 
    soundEngine->SetParameter(nameID, nameParameter, value);
}

/**
 * Se cambia la posicion desde donde se escucha un sonido.
 * TO-DO: Aqui solo se cambia la posicion, para el efecto Doppler hace falta la velocidad. Creo que hay mas cosas a parte.
 */
void SoundFacadeFMOD::SetEventPositionEstatic3D(const string& nameID, const glm::vec3& pos) {
    soundEngine->Set3DAttributes(nameID, pos, 0.0);
}

/**
 * Se cambia la posicion desde donde se escucha un sonido.
 * TO-DO: Aqui solo se cambia la posicion, para el efecto Doppler hace falta la velocidad. Creo que hay mas cosas a parte.
 */
void SoundFacadeFMOD::SetEventPositionDinamic3D(const string& nameID, const glm::vec3& pos, const float vel) {
    soundEngine->Set3DAttributes(nameID, pos, vel);
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
 * Ejecuta el evento de sonido estableciendo un volumen.
 * TO-DO: Actualmente no se puede crear una instancia del mismo evento porque el ID es el mismo.
 * @param nameID - Identificador del sonido en el mapa de instancias.
 */
void SoundFacadeFMOD::PlayEventWithSpecificVolume(const string& nameID, float v) {
    soundEngine->PlayEventWithVolume(nameID, v);
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
    for(auto car : e) {
        auto cId = static_cast<CId*>(car->GetComponent(CompType::IdComp).get());
        auto cPos = static_cast<CTransformable*>(car->GetComponent(CompType::TransformableComp).get());
        auto cCar = static_cast<CCar*>(car->GetComponent(CompType::CarComp).get());
        if(cPos && cId && cCar) {
            string name = "Coche/motor" + to_string(cId->id);
            SetEventPositionDinamic3D(name, cPos->position, cCar->speed);
            SetParameter(name, "velocidad", cCar->speed);
            name = "PowerUp/escudo" + to_string(cId->id);
            SetEventPositionDinamic3D(name, cPos->position, cCar->speed);
            //name = "Coche/derrape" + to_string(cId->id);

        }
    }
}

/**
 *
 */
 void SoundFacadeFMOD::UpdatePowerUps(const vector<shared_ptr<Entity> > &powerups) {
    string name = "";
    for(auto pu : powerups) {
        auto cId = static_cast<CId*>(pu->GetComponent(CompType::IdComp).get());
        auto cPos = static_cast<CTransformable*>(pu->GetComponent(CompType::TransformableComp).get());
        auto cType = static_cast<CPowerUp*>(pu->GetComponent(CompType::PowerUpComp).get());
        if(cPos && cId && cType) {
            //cout << "Actualizo el PU: " << cId->id << endl;
            if (int(cType->typePowerUp) == 3) {
                name = "PowerUp/pudin" + to_string(cId->id);
            } else if (int(cType->typePowerUp) == 5) {
                name = "PowerUp/telebanana_prov" + to_string(cId->id);
            } else if (int(cType->typePowerUp) == 6) {
                name = "PowerUp/melonmolon" + to_string(cId->id);
            }
            soundEngine->Set3DAttributes(name, cPos->position, 0.0);
        }
    }
 }

 /**
  *
  */
  void SoundFacadeFMOD::UpdateTotem(const vector<shared_ptr<Entity> > &totems) {
     for(auto t : totems) {
         auto cId = static_cast<CId*>(t->GetComponent(CompType::IdComp).get());
         auto cPos = static_cast<CTransformable*>(t->GetComponent(CompType::TransformableComp).get());
         if(cPos && cId) {
             string name = "Partida/totem" + to_string(cId->id);
             soundEngine->Set3DAttributes(name, cPos->position, 0.0);
             //cout << "ACTUALIZO POS TOTEM A: " << cPos->position.x << " - " << cPos->position.z << endl;
         }
     }
  }

 /**
  *
  */
 void SoundFacadeFMOD::UpdateListener(const shared_ptr<CarHuman> &mainCar) {
    //cout << "############# UPDATE LISTENER" << endl;
    auto cTrans = static_cast<CTransformable*>(mainCar->GetComponent(CompType::TransformableComp).get());
    auto cId = static_cast<CId*>(mainCar->GetComponent(CompType::IdComp).get());
    if(cTrans && cId) {
        soundEngine->SetListenerPosition(cTrans->position, cTrans->rotation);
    }
 }

/**
 * Actualiza la fachada de sonido. 
 */
void SoundFacadeFMOD::Update() {
    soundEngine->UpdateEngine();

}


/**
 * Cosicas pa cuando la partida empieza
 */

void SoundFacadeFMOD::StartGame() {
    PlayEvent("Ambiente/ambiente");
    PlayEvent("Musica/in_game_1");
    //srand(time(nullptr));
    //character = rand() % 5;
    //cout << "++++ Personaje en sonido: " << character << endl;
    SetParameter("Personajes/voces", "personaje", character);
    SetParameter("Coche/claxon", "personaje", character);
}

/*
 ***************************************************************************
 * EVENTOS
 * *************************************************************************
*/

// -------------------------------------------------- AJUSTES
void SoundFacadeFMOD::SetGlobalVolume(DataMap* d) { 
    auto volume = any_cast<float>((*d)[NUM]);
    soundEngine->SetGlobalVolume(volume);
}

// -------------------------------------------------- SELECCION

void SoundFacadeFMOD::SoundRandomSentence(DataMap* d) {
    auto cPersonaje = any_cast<int>((*d)[NUM]);
    
    SetParameter("Personajes/voces", "personaje", cPersonaje);
    SetParameter("Personajes/voces", "Tipo", TipoVoz::Seleccion);
    PlayEvent("Personajes/voces");
}

void SoundFacadeFMOD::SetCharacter(DataMap* d) {
    auto cPersonaje = any_cast<int>((*d)[NUM]);
    //cout << "++++++++++++++++++ PERSONAJE: " << cPersonaje << endl;
    character = cPersonaje;
}

// --------------------------------------------------

void SoundFacadeFMOD::SoundClaxon(DataMap* d) {
    PlayEvent("Coche/claxon");
}                   

void SoundFacadeFMOD::SoundHurt(DataMap* d) {           
    auto mainCharacter = any_cast<bool>((*d)[MAIN_CAR]);
    auto position = any_cast<glm::vec3>((*d)[VEC3_POS]);
    auto id = any_cast<uint16_t>((*d)[ID]);

    if (mainCharacter) {
        SetParameter("Personajes/voces", "Tipo", TipoVoz::ChoquePowerup);
        PlayEvent("Personajes/voces");
    }
    string nameEvent = "Coche/choque_powerup" + to_string(id);
    SetEventPositionEstatic3D(nameEvent, position);
    PlayEvent(nameEvent);
}                    

void SoundFacadeFMOD::SoundCatchTotem(DataMap* d) {
    auto position = any_cast<glm::vec3>((*d)[VEC3_POS]);
    string mapID = "Partida/coger_totem0";
    SetEventPositionEstatic3D(mapID, position);
    PlayEvent(mapID);
}                    

void SoundFacadeFMOD::SoundCrash(DataMap* d) {      
    bool mainCharacter = any_cast<bool>((*d)[MAIN_CAR]);
    auto id = any_cast<uint16_t>((*d)[ID]);
    auto pos = any_cast<glm::vec3>((*d)[VEC3_POS]);

    if (mainCharacter) {
        SetParameter("Personajes/voces", "Tipo", TipoVoz::ChoqueEnemigo);
        PlayEvent("Personajes/voces");
    }
    string mapID = "Coche/choque" + to_string(id);
    if (!soundEngine->IsPlayingEstatic3D(mapID)) {
        SetEventPositionEstatic3D(mapID, pos);
        PlayEvent(mapID);
    }
}

void SoundFacadeFMOD::SoundCrashWall(DataMap* d) {

    auto id = any_cast<uint16_t>((*d)[ID]);
    auto pos = any_cast<glm::vec3>((*d)[VEC3_POS]);

    string name = "Coche/choque" + to_string(id);
    if (!soundEngine->IsPlayingEstatic3D(name)) {
        SetEventPositionEstatic3D(name, pos);
        PlayEvent(name);
    }
}

void SoundFacadeFMOD::SoundBreakBox(DataMap* d) {                            
    auto idEntity = any_cast<uint16_t>((*d)[ID]);
    string mapID = "Partida/coger_caja" + to_string(idEntity);
    PlayEvent(mapID);
}                    

void SoundFacadeFMOD::SoundDrift(DataMap* d) {
    /*eventInstances3DD["Coche/derrape"] = CreateInstance("Coche/derrape");
    //SetParameter("Personajes/voces", "Tipo", TipoVoz::Derrape);
    PlayEvent("Coche/derrape");*/
}




void SoundFacadeFMOD::SoundMenuOption(DataMap* d) {
    PlayEvent("Menu/cambio_opcion");
}

void SoundFacadeFMOD::SoundMenuOk(DataMap* d) {
    PlayEvent("Menu/aceptar");
}

void SoundFacadeFMOD::SoundMenuBack(DataMap* d) {
    PlayEvent("Menu/atras");
}


void SoundFacadeFMOD::SoundThrowPowerup(DataMap* d) {
    auto cId = any_cast<uint16_t>((*d)[ID]);
    auto typepw = any_cast<typeCPowerUp>((*d)[TYPE_POWER_UP]);
    auto mainCar = any_cast<bool>((*d)[MAIN_CAR]);
    auto cPos = any_cast<glm::vec3>((*d)[VEC3_POS]);
    //cout << "DEBUGEAO LA POS: " << cPos.x << " - " << cPos.y << " - " << cPos.z << endl;

    string name;

    switch (typepw) {
        case typeCPowerUp::RoboJorobo: {
            if( any_cast<bool>((*d)[STOLE]) ) {
                name = "Partida/coger_totem0";
                SetEventPositionEstatic3D(name, cPos);
                PlayEvent(name);
            }
            name = "PowerUp/robojorobo";
            if (mainCar) {
                PlayEventWithSpecificVolume("PowerUp/robojorobo", 1.0);
            } else {
                PlayEventWithSpecificVolume("PowerUp/robojorobo", 0.35);
            }
            break;
        }
        case typeCPowerUp::SuperMegaNitro: {
            if (mainCar) {
                SetParameter("Personajes/voces", "Tipo", TipoVoz::Nitro);
                PlayEvent("Personajes/voces");
            }
            break;
        }
        case typeCPowerUp::EscudoMerluzo: {
            name = "PowerUp/escudo" + to_string(cId);
            PlayEvent(name);
            break;
        }
        case typeCPowerUp::MelonMolon: {
            if (mainCar) {
                SetParameter("Personajes/voces", "Tipo", TipoVoz::Powerup);
                PlayEvent("Personajes/voces");
            }
            break;
        }
        case typeCPowerUp::TeleBanana: {
            if (mainCar) {
                SetParameter("Personajes/voces", "Tipo", TipoVoz::Powerup);
                PlayEvent("Personajes/voces");
            }
            break;
        }
        case typeCPowerUp::PudinDeFrambuesa: {
            if (mainCar) {
                SetParameter("Personajes/voces", "Tipo", TipoVoz::Powerup);
                PlayEvent("Personajes/voces");
            }
            break;
        }
        default:
            cout << "***** El powerup " << (int)typepw << " no tiene sonido" << endl;
    }
}

void SoundFacadeFMOD::SoundClock(DataMap* d) {
    if(!soundEngine->IsPlaying2D("Partida/reloj")){
        PlayEvent("Partida/reloj");
    }
}

void SoundFacadeFMOD::SoundVictoryVoice(){
    cout << "+++++++++++++++++ PERSONAJE: " << character << endl;
    SetParameter("Personajes/victoria", "personaje", character);
    if ( GameValues::GetInstance()->GetWin() ) {
        PlayEvent("Personajes/victoria");
    } else {
        PlayEvent("Personajes/derrota");
    }
}

void SoundFacadeFMOD::SoundVictory(DataMap* d){
    SetParameter("Personajes/victoria", "personaje", character);
    PlayEvent("Personajes/victoria");
}

void SoundFacadeFMOD::SoundDefeat(DataMap* d){
    SetParameter("Personajes/victoria", "personaje", character);
    PlayEvent("Personajes/derrota");
}

// -------------> STOP


void SoundFacadeFMOD::StopShield(DataMap* d) {                                //------------------------------------ HECHO
    auto position = any_cast<glm::vec3>((*d)[VEC3_POS]);
    auto id = any_cast<uint16_t>((*d)[ID]);
    string mapID = "PowerUp/escudo" + to_string(id);
    StopEvent(mapID);

    mapID = "PowerUp/escudo_roto" + to_string(id);
    SetEventPositionEstatic3D(mapID, position);
    PlayEvent(mapID);
}

void SoundFacadeFMOD::StopDrift(DataMap* d) {
    StopEvent("Coche/derrape");
}

void SoundFacadeFMOD::StopClock(DataMap* d) {
    StopEvent("Partida/reloj");
}

void SoundFacadeFMOD::StopSoundMM(DataMap* d) {
    //cout << "BORRAME MM" << endl;
    auto id = any_cast<uint16_t>((*d)[ID]);
    string name = "PowerUp/melonmolon" + std::to_string(id);
    soundEngine->StopEvent(name);
}

void SoundFacadeFMOD::StopSoundTB(DataMap* d) {
    //cout << "BORRAME TB" << endl;
    auto id = any_cast<uint16_t>((*d)[ID]);
    string name = "PowerUp/telebanana" + std::to_string(id);
    soundEngine->StopEvent(name);
}