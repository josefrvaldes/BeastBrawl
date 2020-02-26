#include "SoundEngine.h"

using namespace CLSE;

/*
 * FMOD ERRORS
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
 * SOUND FACADE FMOD
 */

SoundEngine::SoundEngine() {
    InitSoundEngine();
    LoadMasterBank();
    std::cout << "***** Sound engine ON" << endl;
}

SoundEngine::~SoundEngine() {
    delete coreSystem;
    delete system;
    delete masterBank;
    delete stringsBank;
}


/*
 * Inicializa FMOD Studio, inicializando tambien FMOD Core.
 */
void SoundEngine::InitSoundEngine() {
    ERRFMODCHECK(FMOD::Studio::System::create(&system));
    ERRFMODCHECK(system->getCoreSystem(&coreSystem));
    ERRFMODCHECK(coreSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0));
    ERRFMODCHECK(system->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0));

    LoadMasterBank();
    std::cout << "***** Sound engine ON" << endl;
}

/*
 * Libera los audios y bancos de sonido.
 */
void SoundEngine::TerminateSoundEngine() {
    UnloadAllBanks();
    UnloadMasterBank();

    ERRFMODCHECK(system->release());
    std::cout << "***** Sound engine K.O." << endl;
}

/*
 * Carga el banco Master y el MasterStrings.
 */
void SoundEngine::LoadMasterBank() {
    ERRFMODCHECK(system->loadBankFile("./media/fmod/Master.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank));
    ERRFMODCHECK(system->loadBankFile("./media/fmod/Master.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank));
}

/*
 * Libera el banco Master y MasterStrings.
*/
void SoundEngine::UnloadMasterBank() {
    ERRFMODCHECK(stringsBank->unload());
    ERRFMODCHECK(masterBank->unload());
    std::cout << "***** Master bank a la shit" << endl;
}

/*
 * Libera los sonidos que se han cargado en memoria y los bancos.
 * TO-DO: Sacar una funcion para hacer lo mismo. UnloadBank.
 */
void SoundEngine::UnloadAllBanks() {
    /*for ( auto it = eventInstances2D.begin(); it != eventInstances2D.end(); it++){
        ERRFMODCHECK(it->second->release());
    }
    eventInstances2D.clear();

    for ( auto it = eventInstancesEstatic3D.begin(); it != eventInstancesEstatic3D.end(); it++){
        ERRFMODCHECK(it->second->release());
    }
    eventInstancesEstatic3D.clear();
    
    for ( auto it = eventInstancesDinamic3D.begin(); it != eventInstancesDinamic3D.end(); it++){
        ERRFMODCHECK(it->second->release());
    }
    eventInstancesDinamic3D.clear();

    soundDescriptions.clear();

    for (auto bank : banks) {
        ERRFMODCHECK(bank.second->unload());
    }
    banks.clear();*/
    std::cout << "***** Bancos y sonidos liberados" << endl;
}

void SoundEngine::CreateSoundNode2D(const string& nameEvent) {
    unique_ptr<SoundNode> snode = make_unique<SoundNode>();

    FMOD::Studio::EventInstance* instance = nullptr;
    auto description = soundDescriptions.find(nameEvent);
    if (description != soundDescriptions.end()) {
        ERRFMODCHECK(description->second->createInstance(&instance));
        snode.get()->SetInstance(*instance);
        eventInstances2D[nameEvent] = move(snode);
    }
}

void SoundEngine::CreateSoundNodeEstatic3D(uint16_t idE, glm::vec3& p, string& nameEvent) {
    std::string name = nameEvent + to_string(idE);
    unique_ptr<SoundNode> snode = make_unique<SoundNode>(idE, p);
    
    FMOD::Studio::EventInstance* instance = nullptr;
    auto description = soundDescriptions.find(nameEvent);
    if (description != soundDescriptions.end()) {
        ERRFMODCHECK(description->second->createInstance(&instance));
        snode.get()->SetInstance(*instance);
        eventInstancesEstatic3D[name] = move(snode);
    }
}

void SoundEngine::CreateSoundNodeDinamic3D(uint16_t idE, glm::vec3& p, string& nameEvent) {
    std::string name = nameEvent + to_string(idE);
    unique_ptr<SoundNode> snode = make_unique<SoundNode>(idE, p);
    
    FMOD::Studio::EventInstance* instance = nullptr;
    auto description = soundDescriptions.find(nameEvent);
    if (description != soundDescriptions.end()) {
        ERRFMODCHECK(description->second->createInstance(&instance));
        snode.get()->SetInstance(*instance);
        eventInstancesDinamic3D[name] = move(snode);
    }
}


void SoundEngine::UpdateDevice(){

}