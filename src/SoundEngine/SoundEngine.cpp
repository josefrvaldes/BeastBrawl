#include "SoundEngine.h"

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
SoundEngine::SoundEngine() {
    InitSoundEngine(true);
    LoadMasterBank();
    std::cout << "***** Sound engine ON" << endl;
}

SoundEngine::~SoundEngine() {
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
void SoundEngine::InitSoundEngine(bool righthanded) {
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
void SoundEngine::TerminateSoundEngine() {
    UnloadAllBanks();
    UnloadMasterBank();

    //ERRFMODCHECK(system->release());
    ERRFMODCHECK(FMOD_Studio_System_Release(system));
    std::cout << "***** Sound engine K.O." << endl;
}

/**
 * Carga el banco Master y el MasterStrings.
 */
void SoundEngine::LoadMasterBank() {
    //ERRFMODCHECK(system->loadBankFile("./media/fmod/Master.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank));
    ERRFMODCHECK(FMOD_Studio_System_LoadBankFile(system, "./media/fmod/Master.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank));
    ERRFMODCHECK(FMOD_Studio_System_LoadBankFile(system, "./media/fmod/Master.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank));
}

/**
 * Libera el banco Master y MasterStrings.
*/
void SoundEngine::UnloadMasterBank() {
    //ERRFMODCHECK(stringsBank->unload());
    ERRFMODCHECK(FMOD_Studio_Bank_Unload(stringsBank));
    ERRFMODCHECK(FMOD_Studio_Bank_Unload(masterBank));
    std::cout << "***** Master bank a la shit" << endl;
}

/**
 * Libera los sonidos que se han cargado en memoria y los bancos.
 * TO-DO: Sacar una funcion para hacer lo mismo. UnloadBank.
 * TO-DO: ¿Eliminar contenido?
 */
void SoundEngine::UnloadAllBanks() {
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
    std::cout << "***** Bancos y sonidos liberados" << endl;
}

/**
 * Carga el banco y sus respectivos eventos.
 * @param nameBank - Nombre del banco a cargar.
 * @param type - 1 para eventos 3D y 0 para eventos 2D.
 */
bool SoundEngine::LoadSoundBank(const string& nameBank) {
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
void SoundEngine::LoadSoundEvent(const string& nameEvent, const bool type) {
    if (soundDescriptions.find(nameEvent) == soundDescriptions.end()) {
        soundDescriptions[nameEvent] = nullptr;

        string event = "event:/" + nameEvent;
        //ERRFMODCHECK(system->getEvent(event.c_str(), &soundDescriptions[nameEvent]));
        ERRFMODCHECK(FMOD_Studio_System_GetEvent(system, event.c_str(), &soundDescriptions[nameEvent]));

        if (type) {
            //ERRFMODCHECK(soundDescriptions[nameEvent]->loadSampleData());
            ERRFMODCHECK(FMOD_Studio_EventDescription_LoadSampleData(soundDescriptions[nameEvent]));
        } else {
            CreateSoundNode2D(nameEvent);
        }
    }
}

/**
 * Ejecuta el evento de sonido.
 * TO-DO: Actualmente no se puede crear una instancia del mismo evento porque el ID es el mismo.
 * @param nameID - Identificador del sonido en el mapa de instancias.
 */
void SoundEngine::PlayEvent(const string& nameID) {
    //cout << "*** Quiero que suene el evento: " << nameID << endl;
    auto v = globalVolume;
    auto instance = eventInstances2D.find(nameID);
    if (instance != eventInstances2D.end()) {
        SetVolume(instance->second->GetInstance(), globalVolume);
        ERRFMODCHECK(FMOD_Studio_EventInstance_Start(instance->second->GetInstance()));
    } else{
        instance = eventInstancesEstatic3D.find(nameID);
        if(instance != eventInstancesEstatic3D.end()) {
            //cout << "Sonando el evento estatico: " << nameID << endl;
            SetVolume(instance->second->GetInstance(), globalVolume);
            ERRFMODCHECK(FMOD_Studio_EventInstance_Start(instance->second->GetInstance()));
        } else {
            instance = eventInstancesDinamic3D.find(nameID);
            if (instance != eventInstancesDinamic3D.end()) {
                //cout << "Sonando el evento dinamico: " << nameID << endl;
                SetVolume(instance->second->GetInstance(), globalVolume);
                ERRFMODCHECK(FMOD_Studio_EventInstance_Start(instance->second->GetInstance()));
            }
            else {
                cout << "EL EVENTO " << nameID << " NO ESTA" << endl;
            }
        }
    }
}

/**
 * Ejecuta el evento de sonido con un volumen determinado
 * TO-DO: Actualmente no se puede crear una instancia del mismo evento porque el ID es el mismo.
 * @param nameID - Identificador del sonido en el mapa de instancias.
 */
void SoundEngine::PlayEventWithVolume(const string& nameID, float v) {
    auto vol = globalVolume * v;
    auto instance = eventInstances2D.find(nameID);
    if (instance != eventInstances2D.end()) {
        SetVolume(instance->second->GetInstance(), vol);
        ERRFMODCHECK(FMOD_Studio_EventInstance_Start(instance->second->GetInstance()));
    } else{
        instance = eventInstancesEstatic3D.find(nameID);
        if(instance != eventInstancesEstatic3D.end()) {
            //cout << "Sonando el evento estatico: " << nameID << endl;
            SetVolume(instance->second->GetInstance(), vol);
            ERRFMODCHECK(FMOD_Studio_EventInstance_Start(instance->second->GetInstance()));
        } else {
            instance = eventInstancesDinamic3D.find(nameID);
            if (instance != eventInstancesDinamic3D.end()) {
                //cout << "Sonando el evento dinamico: " << nameID << endl;
                SetVolume(instance->second->GetInstance(), vol);
                ERRFMODCHECK(FMOD_Studio_EventInstance_Start(instance->second->GetInstance()));
            }
            else {
                cout << "EL EVENTO " << nameID << " NO ESTA" << endl;
            }
        }
    }
}

/**
 * Para todos los sonidos.
 * @param nameID - Identificador del sonido en el mapa de instancias.
 */
void SoundEngine::StopAllEvents() {
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
void SoundEngine::StopEvent(const string& nameID) {
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
            } else {
                cout << "++++ Sonido " << nameID << " no se ha podido borrar, no esta" << endl;
            }
        }
    }
}



/**
 * Pone en pause todos los sonidos.
 */
void SoundEngine::PauseAllEvents() {
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
void SoundEngine::ResumeAllEvents() {
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
void SoundEngine::PauseEvent(const string& nameID) {
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
void SoundEngine::ResumeEvent(const string& nameID) {
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
 bool SoundEngine::IsPlaying(FMOD_STUDIO_EVENTINSTANCE* instance) const {
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
bool SoundEngine::IsPlaying2D(const string& nameEvent) const {
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
bool SoundEngine::IsPlayingEstatic3D(const string& nameEvent) const {
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
bool SoundEngine::IsPlayingDinamic3D(const string& nameEvent) const {
    auto it = eventInstancesDinamic3D.find(nameEvent);
    if ( it != eventInstancesDinamic3D.end()) {
        return IsPlaying(it->second->GetInstance());
    }
    return false;
}

/**
 * Actualiza el motor de sonido de FMOD.
 */
void SoundEngine::UpdateEngine(){
    //ERRFMODCHECK(system->update());

    for ( auto it = eventInstancesDinamic3D.begin(); it != eventInstancesDinamic3D.end(); ) {
        if ( !IsPlayingDinamic3D(it->first) && it->second->GetClean()) {
            ERRFMODCHECK(FMOD_Studio_EventInstance_Release(it->second.get()->GetInstance()));
            cout << "Se ha borrado la instancia: " << it->first << endl;
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

void SoundEngine::SetGlobalVolume(float gv){
    globalVolume = gv;
    for ( auto it = eventInstances2D.begin(); it != eventInstances2D.end(); it++) {
        SetVolume(it->second->GetInstance(), globalVolume);
    }
}

void SoundEngine::SetVolume(const string& nameID, float v) {
    auto instance = eventInstances2D.find(nameID);
    if (instance != eventInstances2D.end()) {
        SetVolume(instance->second->GetInstance(), v);
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

void SoundEngine::SetVolume(FMOD_STUDIO_EVENTINSTANCE* instance, float v) {
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
void SoundEngine::SetParameter(const string& nameID, const string& nameParameter, const float value) {
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
void SoundEngine::SetListenerPosition(const glm::vec3 &pos, const glm::vec3 &rot) {

    //float rad = glm::radians(rot.y);
    FMOD_VECTOR vec;
    vec.x = pos.x*0.1;
    vec.y = pos.y*0.1;
    vec.z = pos.z*0.1;

    FMOD_3D_ATTRIBUTES atr;
    atr.position = {vec};
    //TODO: Hay que cambiar la direccion boys y poniento la rotación tal cual no funsiona, seguramente porque habrá que cambiar también el UP
    atr.forward = {glm::cos(glm::radians(rot.y)), 0.0, -glm::sin(glm::radians(rot.y))};
    atr.up = {0.0, 1.0, 0.0};
    atr.velocity = {0.0, 0.0, 0.0}; // Para el senior efecto Doppler
    //std::cout << "El forward es: " << glm::cos(glm::radians(rot.y)) << " - " << glm::sin(glm::radians(rot.y)) << std::endl;
    ERRFMODCHECK(FMOD_Studio_System_SetListenerAttributes(system, 0, &atr));
}



/**
 * Se cambia la posicion desde donde se escucha un sonido.
 * TODO: Aqui solo se cambia la posicion, para el efecto Doppler hace falta la velocidad. Creo que hay mas cosas a parte.
 */
void SoundEngine::SetEventPosition3D(FMOD_STUDIO_EVENTINSTANCE * i, const glm::vec3& pos, const float vel) {
    if (i) {
        FMOD_VECTOR vec;
        vec.x = pos.x*0.1;
        vec.y = pos.y*0.1;
        vec.z = pos.z*0.1;

        FMOD_3D_ATTRIBUTES atr;
        atr.position = vec;
        atr.forward = {1.0, 0.0, 0.0};
        atr.up = {0.0, 1.0, 0.0};
        atr.velocity = {vel*0.1f, vel*0.1f, vel*0.1f}; // Para el senior efecto Doppler

        ERRFMODCHECK( FMOD_Studio_EventInstance_Set3DAttributes(i, &atr));
    }
}

/**
 * Cambiamos los parametros de los sonidos 3D
 * @param nameEvent - Nombre de la clave.
 * @param pos - Posicion a la que hay que poner el sonido.
 */
void SoundEngine::Set3DAttributes(const string &nameEvent, const glm::vec3 &pos, const float vel) {
    auto it = eventInstancesEstatic3D.find(nameEvent);
    if (it != eventInstancesEstatic3D.end()) {
        SetEventPosition3D(it->second->GetInstance(), pos, vel);
    } else {
        it = eventInstancesDinamic3D.find(nameEvent);
        if (it != eventInstancesDinamic3D.end()) {
            SetEventPosition3D(it->second->GetInstance(), pos, vel);
        }
    }
}

/*
 ************************************************
 * NODOS SONIDO
 ************************************************
 */

void SoundEngine::CreateSoundNode2D(const string& nameEvent) {
    unique_ptr<SoundNode> snode = make_unique<SoundNode>();

    FMOD_STUDIO_EVENTINSTANCE* instance = nullptr;
    auto description = soundDescriptions.find(nameEvent);
    if (description != soundDescriptions.end()) {
        ERRFMODCHECK( FMOD_Studio_EventDescription_CreateInstance(description->second, &instance));
        snode->SetInstance(*instance);
        eventInstances2D[nameEvent] = move(snode);
    }
}

void SoundEngine::CreateSoundNodeEstatic3D(uint16_t idE, glm::vec3& p, string& nameEvent, bool play) {
    std::string name = nameEvent + std::to_string(idE);
    unique_ptr<SoundNode> snode = make_unique<SoundNode>(idE, p, 0);
    
    FMOD_STUDIO_EVENTINSTANCE * instance = nullptr;
    auto description = soundDescriptions.find(nameEvent);
    if (description != soundDescriptions.end()) {
        ERRFMODCHECK( FMOD_Studio_EventDescription_CreateInstance(description->second, &instance));
        snode->SetInstance(*instance);
        eventInstancesEstatic3D[name] = move(snode);
        SetEventPosition3D(eventInstancesEstatic3D[name].get()->GetInstance(), p, 0.0);
        if (play) {
            PlayEvent(name);
        }
    }
}

void SoundEngine::CreateSoundNodeDinamic3D(uint16_t idE, glm::vec3& p, string& nameEvent, bool play, bool c) {
    std::string name = nameEvent + std::to_string(idE);
    //cout << "Creada instancia: " << name << endl;
    unique_ptr<SoundNode> snode = make_unique<SoundNode>(idE, p, c);
    
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

