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
 * @author Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/
 * 
 */
 
 
 //Made with FMOD Studio by Firelight Technologies Pty Ltd.

#include "SharpEngine.h"

#include <fmod_studio.h>
#include <fmod.h>
#include <fmod_errors.h>

using namespace CLSE;

/*
 ************************************************
 * ERRORES
 ************************************************
 */


#define ERRFMODCHECK(_result) ERRFMODCHECK_fn(_result, __FILE__, __LINE__)

void ERRFMODCHECK_fn(FMOD_RESULT result, const char* file, int line) {
    if (result != FMOD_OK) {
        cerr << file << " (Linea: " << line << "): FMOD HA CASCAO"
             << " -> " << FMOD_ErrorString(result) << " Codigo de error: " << result << endl;
        exit(-1);
    }
}



/*
 ************************************************
 * FMOD
 ************************************************
 */

//TODO: El true no deberia de estar ahi. Inicia a coordenadas diestras.
SharpEngine::SharpEngine() {
    InitSharpEngine(true);
    LoadMasterBank();
    //std::cout << "***** Sound engine ON" << endl;
}

SharpEngine::~SharpEngine() {
    // delete coreSystem;
    // delete system;
    // delete masterBank;
    // delete stringsBank;
    free(coreSystem) ;
    free(system);
    free(masterBank);
    free(stringsBank);
}


/**
 * Inicializa FMOD Studio, inicializando tambien FMOD Core.
 */
void SharpEngine::InitSharpEngine(bool righthanded) {
    //ERRFMODCHECK(FMOD_Studio_System_Create(&system));
    ERRFMODCHECK(FMOD_Studio_System_Create(&system, FMOD_VERSION));
    //ERRFMODCHECK(system->getCoreSystem(&coreSystem));
    ERRFMODCHECK(FMOD_Studio_System_GetCoreSystem(system, &coreSystem));
    //ERRFMODCHECK(coreSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0));
    ERRFMODCHECK(FMOD_System_SetSoftwareFormat(coreSystem, 0, FMOD_SPEAKERMODE_5POINT1, 0));
    //ERRFMODCHECK(system->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr));
    if(righthanded) {
        ERRFMODCHECK(FMOD_Studio_System_Initialize(system, 512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_3D_RIGHTHANDED, nullptr));
    } else {
        ERRFMODCHECK(FMOD_Studio_System_Initialize(system, 512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr));
    }
}

/**
 * Libera los audios y bancos de sonido.
 */
void SharpEngine::TerminateSharpEngine() {
    UnloadAllBanks();
    UnloadMasterBank();

    //ERRFMODCHECK(system->release());
    ERRFMODCHECK(FMOD_Studio_System_Release(system));
    //std::cout << "***** Sound engine K.O." << endl;
}

/**
 * Carga el banco Master y el MasterStrings.
 */
void SharpEngine::LoadMasterBank() {
    //ERRFMODCHECK(system->loadBankFile("./media/fmod/Master.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank));
    ERRFMODCHECK(FMOD_Studio_System_LoadBankFile(system, "./media/fmod/Master.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank));
    ERRFMODCHECK(FMOD_Studio_System_LoadBankFile(system, "./media/fmod/Master.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank));
}

/**
 * Libera el banco Master y MasterStrings.
*/
void SharpEngine::UnloadMasterBank() {
    //ERRFMODCHECK(stringsBank->unload());
    ERRFMODCHECK(FMOD_Studio_Bank_Unload(stringsBank));
    ERRFMODCHECK(FMOD_Studio_Bank_Unload(masterBank));
    //std::cout << "***** Master bank a la shit" << endl;
}

/**
 * Libera los sonidos que se han cargado en memoria y los bancos.
 * TO-DO: Sacar una funcion para hacer lo mismo. UnloadBank.
 * TO-DO: ¿Eliminar contenido?
 */
void SharpEngine::UnloadAllBanks() {
    for ( auto it = eventInstances2D.begin(); it != eventInstances2D.end(); it++){
        //ERRFMODCHECK(it->second.get()->GetInstance()->release());
        ERRFMODCHECK(FMOD_Studio_EventInstance_Release(it->second.get()->GetInstance()));
    }
    eventInstances2D.clear();

    for ( auto it = eventInstancesEstatic3D.begin(); it != eventInstancesEstatic3D.end(); it++){
        ERRFMODCHECK(FMOD_Studio_EventInstance_Release(it->second.get()->GetInstance()));
    }
    eventInstancesEstatic3D.clear();
    
    for ( auto it = eventInstancesDinamic3D.begin(); it != eventInstancesDinamic3D.end(); it++){
        ERRFMODCHECK(FMOD_Studio_EventInstance_Release(it->second.get()->GetInstance()));
    }
    eventInstancesDinamic3D.clear();

    soundDescriptions.clear();

    for (auto bank : banks) {
        ERRFMODCHECK(FMOD_Studio_Bank_Unload(bank.second));
    }
    banks.clear();
    //std::cout << "***** Bancos y sonidos liberados" << endl;
    //std::cout << "   ** EventInstances2D: " << eventInstances2D.size() << endl;
    //std::cout << "   ** EventInstances3DE: " << eventInstancesEstatic3D.size() << endl;
    //std::cout << "   ** EventInstances3DD: " << eventInstancesDinamic3D.size() << endl;
    //std::cout << "   ** Descriptions: " << soundDescriptions.size() << endl;
    //std::cout << "   ** Banks: " << banks.size() << endl;
}

/**
 * Carga el banco y sus respectivos eventos.
 * @param nameBank - Nombre del banco a cargar.
 * @param type - 1 para eventos 3D y 0 para eventos 2D.
 */
bool SharpEngine::LoadSoundBank(const string& nameBank) {
    if (banks.find(nameBank) == banks.end()) {
        banks[nameBank] = nullptr;

        string bank = "./media/fmod/" + nameBank + ".bank";
        ERRFMODCHECK(FMOD_Studio_System_LoadBankFile(system, bank.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &banks[nameBank]));
        //cout << "***** Cargado el banco: " << nameBank << endl;
        return true;
    }
    return false;
}

/**
 * Carga los eventos de sonido. Los 3D sin instancias y los 2D con instancia.
 * @param nameEvent - Identificacion del evento en FMOD Studio.
 * @param type - 3D es 1 y 2D es 0
 */
void SharpEngine::LoadSoundEvent(const string& nameEvent, const bool type, const bool music) {
    if (soundDescriptions.find(nameEvent) == soundDescriptions.end()) {
        soundDescriptions[nameEvent] = nullptr;

        string event = "event:/" + nameEvent;
        //ERRFMODCHECK(system->getEvent(event.c_str(), &soundDescriptions[nameEvent]));
        ERRFMODCHECK(FMOD_Studio_System_GetEvent(system, event.c_str(), &soundDescriptions[nameEvent]));

        if (type) {
            //ERRFMODCHECK(soundDescriptions[nameEvent]->loadSampleData());
            ERRFMODCHECK(FMOD_Studio_EventDescription_LoadSampleData(soundDescriptions[nameEvent]));
        } else {
            if (music){
                CreateSoundNode2D(nameEvent, true);
            } else {
                CreateSoundNode2D(nameEvent, false);
            }
        }
    }
    //cout << "***** Cargado el evento: " << nameEvent << endl;
}

/**
 * Ejecuta el evento de sonido.
 * TO-DO: Actualmente no se puede crear una instancia del mismo evento porque el ID es el mismo.
 * @param nameID - Identificador del sonido en el mapa de instancias.
 */
void SharpEngine::PlayEvent(const string& nameID) {
    //cout << "*** Quiero que suene el evento: " << nameID << endl;
    //auto v = globalVolume;
    auto instance = eventInstances2D.find(nameID);
    if (instance != eventInstances2D.end()) {
        if (instance->second->GetMusic()) {
            SetVolume(instance->second->GetInstance(), musicVolume);
        } else {
            SetVolume(instance->second->GetInstance(), 1.0);
        }
        ERRFMODCHECK(FMOD_Studio_EventInstance_Start(instance->second->GetInstance()));
    } else{
        instance = eventInstancesEstatic3D.find(nameID);
        if(instance != eventInstancesEstatic3D.end()) {
            //cout << "Sonando el evento estatico: " << nameID << endl;
            SetVolume(instance->second->GetInstance(), 1.0);
            ERRFMODCHECK(FMOD_Studio_EventInstance_Start(instance->second->GetInstance()));
        } else {
            instance = eventInstancesDinamic3D.find(nameID);
            if (instance != eventInstancesDinamic3D.end()) {
                //cout << "Sonando el evento dinamico: " << nameID << endl;
                SetVolume(instance->second->GetInstance(), 1.0);
                ERRFMODCHECK(FMOD_Studio_EventInstance_Start(instance->second->GetInstance()));
            }
            //else {
                //cout << "EL EVENTO " << nameID << " NO ESTA" << endl;
            //}
        }
    }
}

/**
 * Ejecuta el evento de sonido con un volumen determinado
 * TO-DO: Actualmente no se puede crear una instancia del mismo evento porque el ID es el mismo.
 * @param nameID - Identificador del sonido en el mapa de instancias.
 */
void SharpEngine::PlayEventWithVolume(const string& nameID, float v) {
    auto instance = eventInstances2D.find(nameID);
    if (instance != eventInstances2D.end()) {
        if (instance->second->GetMusic()) {
            SetVolume(instance->second->GetInstance(), musicVolume*v);
        } else {
            SetVolume(instance->second->GetInstance(), v);
        }
        ERRFMODCHECK(FMOD_Studio_EventInstance_Start(instance->second->GetInstance()));
    } else{
        instance = eventInstancesEstatic3D.find(nameID);
        if(instance != eventInstancesEstatic3D.end()) {
            //cout << "Sonando el evento estatico: " << nameID << endl;
            SetVolume(instance->second->GetInstance(), v);
            ERRFMODCHECK(FMOD_Studio_EventInstance_Start(instance->second->GetInstance()));
        } else {
            instance = eventInstancesDinamic3D.find(nameID);
            if (instance != eventInstancesDinamic3D.end()) {
                //cout << "Sonando el evento dinamico: " << nameID << endl;
                SetVolume(instance->second->GetInstance(), v);
                ERRFMODCHECK(FMOD_Studio_EventInstance_Start(instance->second->GetInstance()));
            }
            //else {
              //cout << "EL EVENTO " << nameID << " NO ESTA" << endl;
            //}
        }
    }
}

/**
 * Para todos los sonidos.
 * @param nameID - Identificador del sonido en el mapa de instancias.
 */
void SharpEngine::StopAllEvents() {
    for ( auto it = eventInstances2D.begin(); it != eventInstances2D.end(); it++){
        //ERRFMODCHECK(it->second->GetInstance()->stop(FMOD_STUDIO_STOP_IMMEDIATE));
        ERRFMODCHECK(FMOD_Studio_EventInstance_Stop(it->second->GetInstance(), FMOD_STUDIO_STOP_IMMEDIATE));
    }
    for ( auto it = eventInstancesEstatic3D.begin(); it != eventInstancesEstatic3D.end(); it++){
        ERRFMODCHECK(FMOD_Studio_EventInstance_Stop(it->second->GetInstance(), FMOD_STUDIO_STOP_IMMEDIATE));
    }
    for ( auto it = eventInstancesDinamic3D.begin(); it != eventInstancesDinamic3D.end(); it++){
        ERRFMODCHECK(FMOD_Studio_EventInstance_Stop(it->second->GetInstance(), FMOD_STUDIO_STOP_IMMEDIATE));
    }
}

/**
 * Para el evento de sonido.
 * @param nameID - Identificador del sonido en el mapa de instancias.
 */
void SharpEngine::StopEvent(const string& nameID) {
    //cout << nameID << endl;
    auto instance = eventInstances2D.find(nameID);
    if (instance != eventInstances2D.end()) {
        ERRFMODCHECK(FMOD_Studio_EventInstance_Stop(instance->second->GetInstance(), FMOD_STUDIO_STOP_IMMEDIATE));
    } else {
        instance = eventInstancesEstatic3D.find(nameID);
        if (instance != eventInstancesEstatic3D.end()) {
            ERRFMODCHECK(FMOD_Studio_EventInstance_Stop(instance->second->GetInstance(), FMOD_STUDIO_STOP_IMMEDIATE));
        } else {
            instance = eventInstancesDinamic3D.find(nameID);
            if (instance != eventInstancesDinamic3D.end()) {
                ERRFMODCHECK(FMOD_Studio_EventInstance_Stop(instance->second->GetInstance(), FMOD_STUDIO_STOP_IMMEDIATE));
                //cout << "++++ Sonido " << nameID << " ha sido parado con exito" << endl;
            } //else {
                //cout << "++++ Sonido " << nameID << " no se ha podido borrar, no esta" << endl;
            //}
        }
    }
}



/**
 * Pone en pause todos los sonidos.
 */
void SharpEngine::PauseAllEvents() {
    for ( auto it = eventInstances2D.begin(); it != eventInstances2D.end(); it++){
        PauseEvent(it->first);
    }
    for ( auto it = eventInstancesEstatic3D.begin(); it != eventInstancesEstatic3D.end(); it++){
        PauseEvent(it->first);
    }
    for ( auto it = eventInstancesDinamic3D.begin(); it != eventInstancesDinamic3D.end(); it++){
        PauseEvent(it->first);
    }
}

/**
 * Reanuda en pause todos los sonidos.
 */
void SharpEngine::ResumeAllEvents() {
    for ( auto it = eventInstances2D.begin(); it != eventInstances2D.end(); it++){
        ResumeEvent(it->first);
    }
    for ( auto it = eventInstancesEstatic3D.begin(); it != eventInstancesEstatic3D.end(); it++){
        ResumeEvent(it->first);
    }
    for ( auto it = eventInstancesDinamic3D.begin(); it != eventInstancesDinamic3D.end(); it++){
        ResumeEvent(it->first);
    }
}


/**
 * Pone en pause el sonido.
 * @param nameID - Identificador del sonido en el mapa de instancias.
 */
void SharpEngine::PauseEvent(const string& nameID) {
    auto instance = eventInstances2D.find(nameID);
    if (instance != eventInstances2D.end()) {
        ERRFMODCHECK(FMOD_Studio_EventInstance_SetPaused(instance->second->GetInstance(), true));
    } else {
        instance = eventInstancesEstatic3D.find(nameID);
        if (instance != eventInstancesEstatic3D.end()) {
            ERRFMODCHECK(FMOD_Studio_EventInstance_SetPaused(instance->second->GetInstance(), true));
        } else {
            instance = eventInstancesDinamic3D.find(nameID);
            if (instance != eventInstancesDinamic3D.end()) {
                ERRFMODCHECK(FMOD_Studio_EventInstance_SetPaused(instance->second->GetInstance(), true));
            }
        }
    }
}

/**
 * Reanuda el sonido.
 * @param nameID - Identificador del sonido en el mapa de instancias.
 */
void SharpEngine::ResumeEvent(const string& nameID) {
    auto instance = eventInstances2D.find(nameID);
    if (instance != eventInstances2D.end()) {
        ERRFMODCHECK(FMOD_Studio_EventInstance_SetPaused(instance->second->GetInstance(), false));
    } else {
        instance = eventInstancesEstatic3D.find(nameID);
        if (instance != eventInstancesEstatic3D.end()) {
            ERRFMODCHECK(FMOD_Studio_EventInstance_SetPaused(instance->second->GetInstance(), false));
        } else {
            instance = eventInstancesDinamic3D.find(nameID);
            if (instance != eventInstancesDinamic3D.end()) {
                ERRFMODCHECK(FMOD_Studio_EventInstance_SetPaused(instance->second->GetInstance(), false));
            }
        }
    }
}

/**
 *
 */
 bool SharpEngine::IsPlaying(FMOD_STUDIO_EVENTINSTANCE* instance) const {
     if (instance) {
         FMOD_STUDIO_PLAYBACK_STATE eventState;
         ERRFMODCHECK(FMOD_Studio_EventInstance_GetPlaybackState(instance, &eventState));
         if (eventState == FMOD_STUDIO_PLAYBACK_PLAYING || eventState == FMOD_STUDIO_PLAYBACK_STARTING /*|| eventState == FMOD_STUDIO_PLAYBACK_SUSTAINING*/) {
             return true;
         }
     }
    return false;
 }

/**
 * Verifica si la instancia de sonido que le enviamos por parametro esta en PLAY o preparandose para el PLAY.
 * @param instance - Instancia del evento a verificar.
 */
bool SharpEngine::IsPlaying2D(const string& nameEvent) const {
    auto it = eventInstances2D.find(nameEvent);
    if ( it != eventInstances2D.end()) {
        return IsPlaying(it->second->GetInstance());
    }
    return false;
}

/**
 * Verifica si la instancia de sonido que le enviamos por parametro esta en PLAY o preparandose para el PLAY.
 * @param instance - Instancia del evento a verificar.
 */
bool SharpEngine::IsPlayingEstatic3D(const string& nameEvent) const {
    auto it = eventInstancesEstatic3D.find(nameEvent);
    if ( it != eventInstancesEstatic3D.end()) {
        return IsPlaying(it->second->GetInstance());
    }
    return false;
}

/**
 * Verifica si la instancia de sonido que le enviamos por parametro esta en PLAY o preparandose para el PLAY.
 * @param instance - Instancia del evento a verificar.
 */
bool SharpEngine::IsPlayingDinamic3D(const string& nameEvent) const {
    auto it = eventInstancesDinamic3D.find(nameEvent);
    if ( it != eventInstancesDinamic3D.end()) {
        return IsPlaying(it->second->GetInstance());
    }
    return false;
}

/**
 * Actualiza el motor de sonido de FMOD.
 */
void SharpEngine::UpdateEngine(){
    //ERRFMODCHECK(system->update());

    for ( auto it = eventInstancesDinamic3D.begin(); it != eventInstancesDinamic3D.end(); ) {
        if ( !IsPlayingDinamic3D(it->first) && it->second->GetClean()) {
            ERRFMODCHECK(FMOD_Studio_EventInstance_Release(it->second.get()->GetInstance()));
            //cout << "Se ha borrado la instancia: " << it->first << endl;
            it = eventInstancesDinamic3D.erase(it);
        } else {
            ++it;
        }
    }

    //cout << "INSTANCIAS 3DD: " << eventInstancesDinamic3D.size() << endl;

    ERRFMODCHECK(FMOD_Studio_System_Update(system));
}



/*
 ************************************************
 * PARAMETROS, LISTENER, 3D
 ************************************************
 */

void SharpEngine::SetGlobalVolume(float gv){
    globalVolume = gv;
    for ( auto it = eventInstances2D.begin(); it != eventInstances2D.end(); it++) {
        if( it->second->GetMusic()) {
            SetVolume(it->second->GetInstance(), musicVolume);
        } else {
            SetVolume(it->second->GetInstance(), 1.0);
        }
    }
    for ( auto it = eventInstancesEstatic3D.begin(); it != eventInstancesEstatic3D.end(); ++it) {
        SetVolume(it->second->GetInstance(), 1.0);
    }
    for ( auto it = eventInstancesDinamic3D.begin(); it != eventInstancesDinamic3D.end(); ++it) {
        SetVolume(it->second->GetInstance(), 1.0);
    }
}

void SharpEngine::SetMusicVolume(float mv) {
    musicVolume = mv;
    for ( auto it = eventInstances2D.begin(); it != eventInstances2D.end(); ++it) {
        if (it->second->GetMusic()) {
            SetVolume(it->second->GetInstance(), musicVolume);
        }
    }
}

void SharpEngine::SetVolume(const string& nameID, float v) {
    auto instance = eventInstances2D.find(nameID);
    if (instance != eventInstances2D.end()) {
        if (instance->second->GetMusic()) {
            SetVolume(instance->second->GetInstance(), musicVolume*v);
        } else {
            SetVolume(instance->second->GetInstance(), v);
        }
    } else {
        instance = eventInstancesEstatic3D.find(nameID);
        if(instance != eventInstancesEstatic3D.end()) {
            SetVolume(instance->second->GetInstance(), v);
        } else {
            instance = eventInstancesDinamic3D.find(nameID);
            if (instance != eventInstancesDinamic3D.end()) {
                SetVolume(instance->second->GetInstance(), v);
            }
        } 
    }
}

void SharpEngine::SetVolume(FMOD_STUDIO_EVENTINSTANCE* instance, float v) {
    if (instance) {
        ERRFMODCHECK(FMOD_Studio_EventInstance_SetVolume(instance, globalVolume*v));
    }
}

/**
 * Cambia el valor de un parametro del evento pasado por parametro.
 * @param nameID - Identificador del sonido en el mapa de instancias.
 * @param nameParameter - ID del parametro que quiero cambiar del evento.
 * @param value - Valor al que quiero cambiar el parametro.
 */
void SharpEngine::SetParameter(const string& nameID, const string& nameParameter, const float value) {
    //cout << "+++++++" << nameID << " A " << value << endl; 
    auto instance = eventInstances2D.find(nameID);
    if (instance != eventInstances2D.end()) {
        //instance->second->GetInstance()->setParameterByName(nameParameter.c_str(), value);
        ERRFMODCHECK(FMOD_Studio_EventInstance_SetParameterByName(instance->second->GetInstance(), nameParameter.c_str(), value, false));
    } else {
        instance = eventInstancesEstatic3D.find(nameID);
        if(instance != eventInstancesEstatic3D.end()) {
            ERRFMODCHECK(FMOD_Studio_EventInstance_SetParameterByName(instance->second->GetInstance(), nameParameter.c_str(), value, false));
        } else {
            instance = eventInstancesDinamic3D.find(nameID);
            if (instance != eventInstancesDinamic3D.end()) {
                ERRFMODCHECK(FMOD_Studio_EventInstance_SetParameterByName(instance->second->GetInstance(), nameParameter.c_str(), value, false));
            }
        } 
    }
}

/**
 * Cambia de posicion el listener.
 */
void SharpEngine::SetListenerPosition(const glm::vec3 &pos, const glm::vec3 &rot) {

    if (!std::isnan(pos.x) && !std::isnan(pos.y) && !std::isnan(pos.z) && !std::isnan(rot.y)) {
        //float rad = glm::radians(rot.y);
        FMOD_VECTOR vec;
        vec.x = pos.x*0.05;
        vec.y = pos.y*0.05;
        vec.z = pos.z*0.05;

        FMOD_3D_ATTRIBUTES atr;
        atr.position = {vec};
        //TODO: Hay que cambiar la direccion boys y poniento la rotación tal cual no funsiona, seguramente porque habrá que cambiar también el UP
        atr.forward = {glm::cos(glm::radians(rot.y)), 0.0, -glm::sin(glm::radians(rot.y))};
        atr.up = {0.0, 1.0, 0.0};
        atr.velocity = {0.0, 0.0, 0.0}; // Para el senior efecto Doppler
        //std::cout << "El forward es: " << glm::cos(glm::radians(rot.y)) << " - " << glm::sin(glm::radians(rot.y)) << std::endl;
        ERRFMODCHECK(FMOD_Studio_System_SetListenerAttributes(system, 0, &atr));
    } //else if ( std::isnan(pos.x) || std::isnan(pos.y) || std::isnan(pos.z) || std::isnan(rot.y)) {
        //cout << "******* LA POSICION O ROTACION DEL LISTENER HA RECIBIDO UN NaN: [Posicion] " << pos.x << " - " << pos.y << " - " << pos.z << " [Rotacion] " << rot.y << endl;
        //terminate();
    //}
}



/**
 * Se cambia la posicion desde donde se escucha un sonido.
 * TODO: Aqui solo se cambia la posicion, para el efecto Doppler hace falta la velocidad. Creo que hay mas cosas a parte.
 */
void SharpEngine::SetEventPosition3D(const string &nameEvent, FMOD_STUDIO_EVENTINSTANCE * i, const glm::vec3& pos, const float vel) {
    if (!std::isnan(pos.x) && !std::isnan(pos.y) && !std::isnan(pos.z) && !std::isnan(vel) && i) {
        FMOD_VECTOR vec;
        vec.x = pos.x*0.05;
        vec.y = pos.y*0.05;
        vec.z = pos.z*0.05;

        FMOD_3D_ATTRIBUTES atr;
        atr.position = vec;
        atr.forward = {1.0, 0.0, 0.0};
        atr.up = {0.0, 1.0, 0.0};
        atr.velocity = {vel*0.1f, vel*0.1f, vel*0.1f}; // Para el senior efecto Doppler

        ERRFMODCHECK( FMOD_Studio_EventInstance_Set3DAttributes(i, &atr));
    } //else if ( std::isnan(pos.x) || std::isnan(pos.y) || std::isnan(pos.z) || std::isnan(vel)) {
        //cout << "******* ME HA LLEGADO UN VALOR NaN EN EL EVENTO " << nameEvent << ": [Posicion] " << pos.x << " - " << pos.y << " - " << pos.z << " [Velocidad] " << vel << endl;
        //terminate();
    //}
}

/**
 * Cambiamos los parametros de los sonidos 3D
 * @param nameEvent - Nombre de la clave.
 * @param pos - Posicion a la que hay que poner el sonido.
 */
void SharpEngine::Set3DAttributes(const string &nameEvent, const glm::vec3 &pos, const float vel) {
    //cout << "QUIERO CAMBIARLE LOS PARAMETROS 3D A " << nameEvent << " - POS: [" << &pos << "] " << pos.x << " - " << pos.y << " - " << pos.z << " - VEL: " << vel << endl;
    auto it = eventInstancesEstatic3D.find(nameEvent);
    if (it != eventInstancesEstatic3D.end()) {
        SetEventPosition3D(nameEvent, it->second->GetInstance(), pos, vel);
    } else {
        it = eventInstancesDinamic3D.find(nameEvent);
        if (it != eventInstancesDinamic3D.end()) {
            SetEventPosition3D(nameEvent, it->second->GetInstance(), pos, vel);
        }
    }
}

/*
 ************************************************
 * NODOS SONIDO
 ************************************************
 */

void SharpEngine::CreateSoundNode2D(const string& nameEvent, bool music) {
    unique_ptr<SharpNode> snode = make_unique<SharpNode>();

    FMOD_STUDIO_EVENTINSTANCE* instance = nullptr;
    auto description = soundDescriptions.find(nameEvent);
    if (description != soundDescriptions.end()) {
        ERRFMODCHECK( FMOD_Studio_EventDescription_CreateInstance(description->second, &instance));
        snode->SetInstance(*instance);
        snode->SetMusic(music);
        eventInstances2D[nameEvent] = move(snode);
    }
}

void SharpEngine::CreateSoundNodeEstatic3D(uint16_t idE, glm::vec3& p, string& nameEvent, bool play) {
    std::string name = nameEvent + std::to_string(idE);
    unique_ptr<SharpNode> snode = make_unique<SharpNode>(idE, p, 0);
    
    FMOD_STUDIO_EVENTINSTANCE * instance = nullptr;
    auto description = soundDescriptions.find(nameEvent);
    if (description != soundDescriptions.end()) {
        ERRFMODCHECK( FMOD_Studio_EventDescription_CreateInstance(description->second, &instance));
        snode->SetInstance(*instance);
        eventInstancesEstatic3D[name] = move(snode);
        SetEventPosition3D(name, eventInstancesEstatic3D[name].get()->GetInstance(), p, 0.0);
        if (play) {
            PlayEvent(name);
        }
    }
}

void SharpEngine::CreateSoundNodeDinamic3D(uint16_t idE, glm::vec3& p, string& nameEvent, bool play, bool c) {
    std::string name = nameEvent + std::to_string(idE);
    //cout << "Creada instancia: " << name << endl;
    unique_ptr<SharpNode> snode = make_unique<SharpNode>(idE, p, c);
    
    FMOD_STUDIO_EVENTINSTANCE* instance = nullptr;
    auto description = soundDescriptions.find(nameEvent);
    if (description != soundDescriptions.end()) {
        ERRFMODCHECK( FMOD_Studio_EventDescription_CreateInstance(description->second, &instance));
        snode->SetInstance(*instance);
        eventInstancesDinamic3D[name] = move(snode);
        if (play) {
            PlayEvent(name);
        }
    }
}

