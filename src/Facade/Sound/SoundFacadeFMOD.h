#pragma once

#include <iostream>
#include <unordered_map>

#include <fmod_studio.hpp>
#include <fmod.hpp>
#include <fmod_errors.h>

#include "../../EventManager/EventManager.h"
#include "../../EventManager/Event.h"
#include "SoundFacade.h"

using namespace std;

class SoundFacadeFMOD : public SoundFacade {
    public:
        explicit SoundFacadeFMOD() : SoundFacade() {};
        ~SoundFacadeFMOD();

        void InitSoundEngine() override;
        void TerminateSoundEngine() override;
        void LoadMasterBank();
        void UnloadMasterBank();

        void SetState(const uint16_t) override;
        
        void LoadBanksInGame() override;
        void LoadEvent(const char*) override;
        void InsertInstance(const char*, FMOD::Studio::EventInstance*);
        //bool EventIsPlaying(const char*);
        
        void UnloadBank(const char*) override;

        void Update() override;
        //void InitializeState(int state);

        unordered_map<string, FMOD::Studio::EventDescription*> GetDescriptions() { return soundDescriptions;};
        unordered_map<string, FMOD::Studio::EventInstance*> GetInstances() { return eventInstances; }

    private:
        void LoadBanks(const uint16_t) override;
        FMOD::System* coreSystem = NULL;
        FMOD::Studio::System* system = NULL;
        FMOD::Studio::Bank* masterBank = NULL;
        FMOD::Studio::Bank* stringsBank = NULL;

        unordered_map<string, FMOD::Studio::Bank*> banks;
        unordered_map<string, FMOD::Studio::EventDescription*> soundDescriptions;
        unordered_map<string, FMOD::Studio::EventInstance*> eventInstances;
        shared_ptr<EventManager> eventManager;

        void SubscribeToGameEvents(int state);


        // eventos del juego
        void SonarClaxon(Data);
        void SoniditoTecla(Data);
        void VozAlElegirPersonaje(Data);



        // metodos de sonido
        void SonarUnaVez(string);
        void SonarEnBucle(string);
        void SonarOjete(string);
};